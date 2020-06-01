# pragma once
# ifndef ugis_VERTEX_INCLUDED
    # define ugis_VERTEX_INCLUDED
# endif

# include <Siv3D.hpp>

// edgeを描くには2つの頂点の位置が必要
//
//
//
//
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
    extern enum VertexState
    {
        Unsearched = 0,
        Searched = 1,
        Confirmed = 2
    };

    class Vertex2D
    {
    private:
        Vec2 m_position;
        VertexState m_state;
    public:
        Vertex2D() = default;
        Vertex2D(Vec2  , VertexState state = Unsearched);
        Vertex2D(const Vertex& Vertex);
    };
}