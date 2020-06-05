# pragma once
# ifndef ugis_VERTEX_INCLUDED
    # define ugis_VERTEX_INCLUDED
# endif

# include <Siv3D.hpp>

namespace ugis
{     
    /// <summary>
    /// 頂点の状態を表す列挙型
    /// </summary>
    enum class VertexState
    {
        Unsearched = 0,
        Searched = 1,
        Confirmed = 2
    };

    /// <summary>
    /// 二次元グラフ上の頂点
    /// </summary>
    struct Vertex2D
    {   
        using value_type = Vec2;

        value_type position;
        VertexState state;
        
        /// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
        Vertex2D() noexcept = default;
        constexpr Vertex2D(const Vertex2D&) noexcept = default;

        Vertex2D(const Vec2& _position, VertexState _state = VertexState::Unsearched) noexcept 
            : position(_position)
            , state(_state) {}
        
    };
}