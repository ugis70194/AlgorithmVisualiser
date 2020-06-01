# pragma once
# ifndef ugis_GRAPH_INCLUDED
    # define ugis_GRAPH_INCLUDED
# endif

# include <Siv3D.hpp>
# include <Vertex.hpp> 
# include <Edge.hpp>

namespace ugis
{		
	constexpr Vec2 refPoint(50, 50); 
	const size_t width(Window::ClientWidth() - 100);
	const size_t height(Window::ClientHeight() - 100);

	class Graph 
	{		
	private:
		Array<Vec2> m_vertexPositions;
		Array<std::pair<Edge, bool>> m_edges;
		Array<Array<size_t>> m_connectedVertices;
		double m_radius;
		double m_thickness; 
		ColorF m_vertexColor; 
		ColorF m_edgeColor;
		ColorF m_searcedVertexColor;
		ColorF m_searcedEdgeColor;

	public:
		
	};

	Graph& RandomGraph(const size_t& vertex,const size_t& edge)
	{	
		Array<Vec2> vertexPositions;
		Array<Array<size_t>> connectedVertices;
		Array<std::pair<size_t, size_t>> allEdges;
		
		// すべての辺を生成
		for(size_t i = 0; i < vertex; i++) 
		{
			for(size_t k = i+1; k < vertex; k++)
			{
				allEdges << std::make_pair(i, k);
			}
		}

		// 頂点の座標をランダムに決定
		for(size_t i = 0; i < vertex; i++)
		{
			vertexPositions << RandomVec2(RectF(refPoint, width, height));
		}

		// 指定個数だけ辺を選択
		for(size_t i = 0; i < edge; i++)
		{

		}
	}
}