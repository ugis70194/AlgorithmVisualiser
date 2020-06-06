# include<Graph.hpp>

namespace ugis
{
	void Graph::init(size_t width, size_t height, double radius, double thickness, Array<ColorF> vertexColor, Array<ColorF> edgeColor)
	{
		m_vertexColor = vertexColor;
		m_edgeColor = edgeColor;
		m_radius = radius;
		m_thickness = thickness;
		m_height = height;
		m_width = width;

		if (vertex.size() == 0)
		{
			const Vec2 initial(0, 0);
			vertex.assign(connection.size(), Vertex2D(initial));

			{
				Array<Vec2> tmp;
				for (size_t i = 0; i < vertex.size(); i++)
				{
					tmp << RandomVec2(RectF(0, 0, 1920, 1080));
				}

				std::sort(tmp.begin(), tmp.end(), [](Vec2& a, Vec2& b) { return a.x == b.x ? a.x < b.x : a.y < b.y; });
				for (size_t i = 0; i < vertex.size(); i++)
				{
					vertex[i] = tmp[i];
				}
			}

			for (size_t i = 0; i < connection.size(); i++)
			{
				for (const auto& child : connection[i])
				{

					if (edges.count(Edge2D(child, i))) continue;
					edges << Edge2D(i, child);
				}
			}
		}

	}


	bool Graph::draw(bool update, double delay)
	{
		if (edges.size() == 0) return false;
		for (auto& edge : edges)
		{
			size_t state = static_cast<size_t>(edge.state);
			Line(vertex[edge.to].position, vertex[edge.from].position).draw(m_thickness, m_edgeColor[state]);
		}

		for (auto& v : vertex)
		{
			size_t state = static_cast<size_t>(v.state);
			Circle(v.position, m_radius).draw(m_vertexColor[state]);
		}

		if (update)
		{
			auto timer = Timer(delay, true);
			while (!timer.reachedZero()) {}
			return System::Update();
		}
		else return false;
	}
}