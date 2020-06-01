# pragma once
# ifndef ugis_EDGE_INCLUDED
    # define ugis_EDGE_INCLUDED
# endif

# include <Siv3D.hpp>

// edgeを描くには2つの頂点の位置が必要
// edge class objectに頂点の位置を持たせるのはメモリの無駄
// vertexはどうせArray<vertex>で持つ(Graphを作るためのだけのクラスだし)
// それならindex = vertex_idでよくない？
//
//
//
//
//
//
//
//

namespace ugis
{   
    enum EdgeState
    {
        Unsearched = 0,
        Searched = 1,
        Confirmed = 2
    };

    class Edge2D 
    {
    private:
        size_t m_from; // 頂点のArrayでのindex
        size_t m_to;  
        EdgeState m_state;
    public:
        Edge2D() = default;
        Edge2D(size_t from = (size_t)0, size_t to = (size_t)0, EdgeState state = Unsearched);
        Edge2D(const std::pair<size_t, size_t>& vertices, EdgeState state = Unsearched);
        Edge2D(std::pair<size_t, size_t> vertices, EdgeState state = Unsearched);
        Edge2D(const Edge2D& edge);
    };
}