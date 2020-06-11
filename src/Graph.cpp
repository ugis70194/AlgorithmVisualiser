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

		//頂点をランダムに配置
		{	
			Array<Vec2> tmp;
			for ([[maybe_unused]] const auto& i : Range(0, connection.size() - 1))
			{
				tmp << RandomVec2(RectF(m_width*0.2 + m_radius*2, m_radius*2, static_cast<double>(m_width) - m_radius*2, static_cast<double>(m_height) - m_radius*2));
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
	}

	bool Graph::draw(bool update, bool displayEdgeWeight, double delay)
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

		//辺を描画
		for (const auto& edge : edges)
		{
			size_t state = static_cast<size_t>(edge.state);
			const Vec2 s = vertex[edge.to].position;
			const Vec2 e = vertex[edge.from].position;
			const Line line(s, e);

			//頂点が直線上に並んでいるとき、辺を曲げる
			LineString L;
			L << s;
			
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
			
			//辺の重みを描画
			if(!displayEdgeWeight) continue;
			if(L.size() == 2)
			{
				m_font(U"{}"_fmt(edge.weight)).draw(line.center());
			} else {
				m_font(U"{}"_fmt(edge.weight)).draw(L[L.size()/2]);
			}
		}
		
		//頂点を描画
		size_t idx = 0;
		for (const auto& v : vertex)
		{	
			size_t state = static_cast<size_t>(v.state);
			Circle(v.position, m_radius).draw(m_vertexColor[state]);
			m_font(U"{}"_fmt(idx++)).drawAt(v.position, Palette::Black); //頂点番号を描画
		}


		m_viewportRect.drawFrame(0, 2, Palette::Seagreen);
		{	
            // ビューポートの適用
            const ScopedViewport2D viewport(m_viewportRect);
			Rect(0, 0, m_viewportRect.size).draw(ColorF(0.8, 0.9, 1.0));  
        }
		
		//描画の遅延
		if (update)
		{
			auto timer = Timer(delay, true);
			while (!timer.reachedZero()) {}
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
}