# include<Graph.hpp>

namespace ugis
{
	void Graph::init(size_t width, size_t height, double radius, double thickness, Array<ColorF> vertexColor, Array<ColorF> edgeColor, Font font)
	{	
		m_width = width;
		m_height = height;
		m_thickness = thickness;
		m_radius = radius;
		m_vertexColor = vertexColor;
		m_edgeColor = edgeColor;
		m_font = font;
		
		vertex.clear();
		edges.clear();

		{	
			Array<Vec2> tmp;
			for ([[maybe_unused]] const auto& i : Range(0, connection.size() - 1))
			{
				tmp << RandomVec2(RectF(0, 0, static_cast<double>(m_width), static_cast<double>(m_height)));
			}
			std::sort(tmp.begin(), tmp.end(), [](Vec2& a, Vec2& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; });
			for (const auto& v : tmp)
			{
				vertex << v;
			}
		}
		for (const auto& i : Range(0, connection.size() - 1))
		{
			for (const auto& child : connection[i])
			{
				if (edges.count(Edge2D(child, i))) continue;
				edges << Edge2D(i, child);
			}
		}
	}


	bool Graph::draw(bool update, double delay)
	{
		if (vertex.size() == 0) return false;
		if (0 <= m_grab && m_grab < vertex.size())
        {
            // 移動量分だけ円を移動
            vertex[m_grab].position.moveBy(Cursor::Delta());
        }

		for(auto i  : Range(0, vertex.size()))
		{	
			const ugis::Vertex2D v = vertex[i];
			if (Circle(v.position, m_radius).leftClicked()) // 円を左クリックしたら
       	 	{
            	m_grab = static_cast<int32>(i);
        	}
		}
        
        if (MouseL.up()) // マウスの左ボタンが離されたら
        {
            m_grab = -1;
        }
		
		if (edges.size() == 0) return false;
		for (const auto& edge : edges)
		{
			size_t state = static_cast<size_t>(edge.state);
			const Vec2 s = vertex[edge.to].position;
			const Vec2 e = vertex[edge.from].position;
			const Line line(s, e);

			LineString L;
			L << s;
			ClearPrint();
			for (const auto& v : vertex)
			{	
				if(v.position == s || v.position == e) continue;
				const Circle c(v.position, m_radius);
				if(const auto p = c.intersectsAt(line))
				{
					Vec2 x = v.position - s; 
					x = Vec2(x.y, -x.x);
					if(x.length() < 0.001) continue;
					L << v.position + (x*4*m_radius/x.length());
					break;
				}
			}
			L << e;
			L.drawCatmullRom(m_thickness, m_edgeColor[state]);
		}
		

		size_t idx = 0;
		for (const auto& v : vertex)
		{	
			size_t state = static_cast<size_t>(v.state);
			Circle(v.position, m_radius).draw(m_vertexColor[state]);
			m_font(U"{}"_fmt(idx++)).drawAt(v.position, Palette::Black);
		}

		if (update)
		{
			auto timer = Timer(delay, true);
			while (!timer.reachedZero()) {}
			return System::Update();
		}
		return false;
	}
}