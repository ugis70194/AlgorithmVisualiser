
# include<Graph.hpp>


namespace ugis
{	

	void Graph::init(size_t width, size_t height, double radius, double thickness, Font font, Array<ColorF> vertexColor, Array<ColorF> edgeColor)
	{	
		m_width = width;
		m_height = height;
		m_thickness = thickness;
		m_radius = radius;
		m_vertexColor = vertexColor;
		m_edgeColor = edgeColor;
		m_font = font;
		m_viewportRect = Rect(0, 0, 160, static_cast<int32>(m_height));
		m_camera = Camera2D(Vec2(m_width/2, m_height/2), 1.0, Camera2DParameters::MouseOnly());
		
		vertex.clear();
		edges.clear();
		dist.assign(connection.size(), INF);

		//頂点をランダムに配置
		{	
			const double offset_x = 160 + m_radius*2;
			const double offset_y = m_radius*2;
			Array<Vec2> tmp;
			for ([[maybe_unused]] const auto& i : Range(0, connection.size() - 1))
			{
				tmp << RandomVec2(RectF(offset_x, offset_y, static_cast<double>(m_width*3.0) - offset_x, static_cast<double>(m_height*3.0) - offset_y));
			}
			std::sort(tmp.begin(), tmp.end(), [](Vec2& a, Vec2& b) { return a.x == b.x ? a.y < b.y : a.x < b.x; });
			for (const auto& v : tmp)
			{
				vertex << v;
			}
		}
		//辺の情報を保存
		for (const auto& i : Range(0, connection.size() - 1))
		{
			for (const auto& child : connection[i])
			{
				if (edges.count(Edge2D(child, i))) continue;
				edges << Edge2D(i, child);
			}
		}

		m_weightPos.assign(edges.size(), Vec2(0,0));
	}

	void Graph::GUI(bool enable)
	{
		m_GUIEnable = enable;
		if(m_GUIEnable)
		{
			if (vertex.size() == 0) return;
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

			if (KeyUp.pressed())
			{
				m_begin = Max(m_begin - 1, 0);
			}
			if(KeyDown.pressed()) 
			{
				m_begin = Min(m_begin + 1, Max(m_begin, (int32)vertex.size() - 29));
			}
		}
	}

	bool Graph::draw(bool update, bool displayEdgeWeight, double delay, size_t highlight)
	{

		if (edges.size() == 0) return false;
		size_t searchedV = 0;
		size_t searchedE = 0;
		m_camera.update();
		{
			const auto t = m_camera.createTransformer();
			//辺を描画
			for (const auto& edge : edges)
			{	
				if(edge.state != State::Unsearched) ++searchedE;
				size_t state = static_cast<size_t>(edge.state);
				const Vec2 s = vertex[edge.to].position;
				const Vec2 e = vertex[edge.from].position;
				const Line line(s, e);

				//頂点が直線上に並んでいるとき、辺を曲げる
				LineString L;
				L << s;

				//for (const auto& v : vertex)
				//{	
				//	if(v.position == s || v.position == e) continue;
				//	const Circle c(v.position, m_radius);
				//	if(const auto p = c.intersectsAt(line))
				//	{
				//		Vec2 x = v.position - s; 
				//		x = Vec2(x.y, -x.x);
				//		if(x.length() < 0.001) continue;
				//		L << v.position + (x*4*m_radius/x.length());
				//		break;
				//	}
				//}

				L << e;
				L.drawCatmullRom(m_thickness, m_edgeColor[state]);

				//辺の重みを描画
				if(!displayEdgeWeight) continue;
				if(L.size() == 2)
				{	
					m_font(U"{}"_fmt(edge.weight)).draw(line.center(), Palette::White);
				} 
				else 
				{
					m_font(U"{}"_fmt(edge.weight)).draw(L[L.size()/2], Palette::White);
				}
			}

			//頂点を描画
			size_t idx = 0;
			for (const auto& v : vertex)
			{	
				if(v.state != State::Unsearched) ++searchedV;
				size_t state = static_cast<size_t>(v.state);
				Circle(v.position, m_radius).draw(m_vertexColor[state]);
				m_font(U"{}"_fmt(idx++)).drawAt(v.position, Palette::Black); //頂点番号を描画
			}
		}
		//m_camera.draw(Palette::Orange);

		//各頂点までの距離を描画
		{	
            // ビューポートの適用
            const ScopedViewport2D viewport(m_viewportRect);
			Rect(0, 0, m_viewportRect.size).draw(ColorF(U"#1c1c1c"));  

			const int32 begin = m_begin;
			const int32 end = Min((int32)vertex.size(), m_begin + 30);
			const double rectW = 160;
			const double rectH = static_cast<double>(m_height) / 30.0;
			m_f(U"探索済み").draw(RectF(0, 0, rectW, rectH)); 
			m_f(U"頂点数|  {}"_fmt(searchedV)).draw(RectF(0, rectH, rectW, rectH)); 
			m_f(U"辺数|    {}"_fmt(searchedE)).draw(RectF(0, 2*rectH, rectW, rectH)); 
			m_f(U"頂点  距離").draw(RectF(0, 3*rectH, rectW, rectH)); 

			for(auto& i : Range(begin, end - 1))
			{	
				const RectF frame = RectF(0, (i+4 - m_begin)*rectH, rectW, rectH);
				if(highlight == i) frame.drawFrame(1.0, ColorF(U"#ECDC00"));
				m_f(U"{}|   {}"_fmt(i, (dist[i] == INF ? U"NaN" : Format(dist[i])))).draw(frame);
			}
        }
		
		GUI(m_GUIEnable);
		//描画の遅延
		if (update)
		{	
			if(delay > 0.01) [[maybe_unused]] bool b = System::Update();
			auto timer = Timer(delay, true);
			while (!timer.reachedZero()) { draw(true, false, 0.0); }
			if(delay > 0.01) return true;
			return System::Update();
		}
		return true;
	}


	template<class T>
	Array<T> parse(String& S)
	{	
		S.push_back(U' ');
		Array<T> res;
		String tmp = U"";
		for(const auto& c : S)
		{	
			if(c == U' ')
			{		
				res << Parse<T>(tmp);
				tmp = U"";
				continue;
			}
			tmp << c;
		}

		return res;
	}

	void Graph::load(String path)
	{
		TextReader reader(path);
		if (!reader)
    	{
        	throw Error(U"Failed to open");
    	}	

		
		String line;
		reader.readLine(line);
		Array<size_t> VES = parse<size_t>(line);
		const size_t vertexCount = VES[0];
		Array<Array<ListType>> input;
		
		for([[maybe_unused]] auto& i : Range(1, vertexCount))
		{
			input << Array<ListType>();
		}

		while(reader.readLine(line))
		{	
			const Array<int32> edge = parse<int32>(line);
			const size_t from = static_cast<size_t>(edge[0]);
			const size_t to = static_cast<size_t>(edge[1]);
			const int32 weight = edge[2];
			input[from] << ListType(to, weight);
		}

		connection = input;
		init();
	}

	void Graph::BFS(size_t start, size_t goal, double delay)
	{	
		std::queue<size_t> que;
		que.push(start);
		dist[start] = 0;
		vertex[start].state = State::Searched;
		while(!que.empty())
		{
			size_t now = que.front();
			que.pop();
			for(const auto& nxt : connection[now])
			{
				if(vertex[nxt.first].state != ugis::State::Unsearched) continue;
				vertex[nxt.first].state = ugis::State::Searched;
				ugis::Edge2D tmp(nxt.first, now);
				for(auto& edge : edges) if(edge == tmp) edge.state = State::Searched;
				dist[nxt.first] = dist[now] + nxt.second; 
				que.push(nxt.first);
				if(nxt.first == goal) que = std::queue<size_t>();
				draw(true, false, delay, nxt.first);
			}
		}
		for(Vertex2D& v : vertex)
		{
			if(v.state == State::Searched)
			{
				v.state = State::Confirmed;
			}
		}
		for(Edge2D& e : edges)
		{
			if(e.state == State::Searched)
			{
				e.state = State::Confirmed;
			}
		}
	}
}