# include <Siv3D.hpp>

// 色付きの円
struct CircleItem 
{
	double radius;
	Vec2 center;
	ColorF color;

	CircleItem() = default;
	CircleItem(const Vec2& center_, const double& radius_, const ColorF& color_) 
		: center(center_)
		, radius(radius_)
		, color(color_) {}
	
	void draw() const 
	{
		Circle(center, radius).draw(color);
	}
};


///////////  最小包含円  ////////////
//最小包含円を三分探索で求める。
//アイデア:
//	最小包含円は、包含円の中で半径が最も小さいものである。
//  包含円の半径を "円の中心 と 円の中心から最も遠い点までの距離" とすると、これを最小化すれば最小包含円を得られる。


CircleItem MinimumInclusionCircle(Array<Vec2>& points, bool anime = false)
{	
	Vec2 center(0, 0);
	double radius = 10.0;
	CircleItem circle(center, radius, ColorF(0.8, 0.2, 0,2));

	//  点と点の距離
	auto Dist = [&](const Vec2& a, const Vec2& b) -> double
	{	
		const double x_diff = a.x - b.x;
		const double y_diff = a.y - b.y;

		return Sqrt(x_diff*x_diff + y_diff*y_diff);
	};

	//  円の中心 と 円の中心から最も遠い点までの距離
	auto MaxDist = [&](const Vec2& center) -> double 
	{
		double max_dist = 0;
		for(const auto& point : points)
		{
			max_dist = Max(max_dist, Dist(center, point));
		}

		return max_dist;
	};

	//  y座標の三分探索
	auto y = [&](const double x, bool update) -> double 
	{	
		double lower = 50, upper = 850;
		for(int32 i = 0; i < 10; i++)
		{
			double left_y = (lower*2 + upper) / 3.0;
			double right_y = (lower + upper*2) / 3.0;

			Vec2 center_l(x, left_y);
			Vec2 cneter_r(x, right_y);
			Vec2 center_y;

			if(MaxDist(center_l) > MaxDist(cneter_r)) 
			{
				lower = left_y;
				center_y = cneter_r;
			}
			else 
			{
				upper = right_y;
				center_y = center_l;
			}

			//  描画関数
			if(update && anime) 
			{
				circle = CircleItem(center_y, MaxDist(center_y)+10.0, Color(U"#3c3c66"));
				circle.draw();
				for(const auto& point : points)
				{
					Circle(point, 10).draw(Color(U"#cccc33"));
				}
				
				auto timer = Timer(0.6, true);
				while(!timer.reachedZero()) {}
				if(System::Update()) {}
			}
		}

		return (lower + upper) / 2.0;
	};
	
	//  x座標の三分探索
	double lower = 170, upper = 1110;
	for(int32 i = 0; i < 12; i++)
	{
		double left_x = (lower*2 + upper) / 3.0;
		double right_x = (lower + upper*2) / 3.0;

		Vec2 center_l(left_x, y(left_x, 0));
		Vec2 center_r(right_x, y(right_x, 0));

		if(MaxDist(center_l) > MaxDist(center_r)) 
		{
			lower = left_x;
			y(upper, (i&1));
		}
		else 
		{
			upper = right_x;
			y(lower, (i&1));
		}
	}

	double x = (lower + upper) / 2.0;
	center = Vec2(x, y(x, 0));
	radius = MaxDist(center);

	circle = CircleItem(center, radius+10.0, Color(U"#5c5c88"));

	return circle;
}

//  点が円に内包されているか
bool IsInclusion(const CircleItem& circle, const Vec2 point)
{	
	double x_diff = circle.center.x - point.x;
	double y_diff = circle.center.y - point.y;
	double dist = Sqrt(Pow(x_diff, 2) + Pow(y_diff, 2));
	
	return circle.radius >= dist; 
}

//  使用例
void MainExample()
{	
	Scene::SetBackground(Color(U"#1f1e33"));
	Window::Resize(Size(1280, 960));
	Array<Vec2> points;
	CircleItem minimum_inclusion_circle(Vec2(0,0), 0, Color(U"#FFFFFF"));

	constexpr RectF circle(340, 230, 600, 500);
	for (size_t i = 0; i < 50; i++) 
	{
		points << RandomVec2(circle);
	}
	for (const auto& point : points) 
	{
		Circle(point, 10).draw();
	}

	while (System::Update())
	{
		if (Scene::Rect().leftClicked())
		{	
			Vec2 point(Cursor::Pos());
			points << point;
			if(!IsInclusion(minimum_inclusion_circle, point))
			{
				minimum_inclusion_circle = MinimumInclusionCircle(points);
			}
		}

		minimum_inclusion_circle.draw();
		for (const auto& point : points)
		{
			Circle(point, 10).draw(Color(U"#cccc33"));
		}
		

		if (KeyEscape.down())
		{
			System::Exit();
		}
	}
}