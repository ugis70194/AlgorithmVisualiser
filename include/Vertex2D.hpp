# pragma once
# ifndef ugis_VERTEX_INCLUDED
    # define ugis_VERTEX_INCLUDED
# endif

# include <Siv3D/Vector2D.hpp>
using namespace s3d;
# include <State.hpp>

namespace ugis
{     

    /// <summary>
    /// 二次元グラフ上の頂点
    /// </summary>
    struct Vertex2D
    {   
        using value_type = Vec2;

        value_type position;
        State state;
        
        /// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
        Vertex2D() noexcept = default;
        constexpr Vertex2D(const Vertex2D&) noexcept = default;

        Vertex2D(const Vec2& _position,const State _state = State::Unsearched) noexcept 
            : position(_position)
            , state(_state) {}
        
    };
}