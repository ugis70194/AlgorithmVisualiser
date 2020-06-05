# pragma once
# ifndef ugis_EDGE2D_INCLUDED
    # define ugis_EDGE2D_INCLUDED
# endif

# include <Siv3D.hpp>

// edgeを描くには2つの頂点の位置が必要
// edge class objectに頂点の位置を持たせるのはメモリの無駄
// vertexはどうせArray<vertex>で持つ(Graphを作るためのだけのクラスだし)
// それならindex = vertex_idでよくない？

namespace ugis
{       

    /// <summary>
    /// 辺の状態を表す
    /// </summary>
    enum class EdgeState
    {
        Unsearched = 0,
        Searched = 1,
        Confirmed = 2
    };

    /// <summary>
    /// ２次元グラフ上の辺
    /// </summary>
    struct Edge2D 
    {   
        using value_type = size_t;

        value_type from; 
        value_type to;  
        EdgeState state; 

        /// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
        Edge2D() noexcept = default;
        constexpr Edge2D(const Edge2D&) noexcept = default; 

        constexpr Edge2D(value_type _from, value_type _to, const enum EdgeState _state = EdgeState::Unsearched) noexcept
            : from(_from)
            , to(_to)
            , state(_state) {}

        template<class X, class Y, std::enable_if_t< std::is_integral_v<X> && std::is_integral_v<Y> >* = nullptr>
        constexpr Edge2D(X _from, Y _to, const enum EdgeState _state = EdgeState::Unsearched) noexcept
            : from(static_cast<value_type>(_from))
            , to(static_cast<value_type>(_to))
            , state(_state) {}

        template<class X, class Y, std::enable_if_t< std::is_integral_v<X> && std::is_integral_v<Y> >* = nullptr>
        Edge2D(const std::pair<X, Y>& vertices, enum EdgeState _state = EdgeState::Unsearched) noexcept
            : from(static_cast<value_type>(vertices.first))
            , to(static_cast<value_type>(vertices.second))
            , state(_state) {}

        template<class X, class Y, std::enable_if_t< std::is_integral_v<X> && std::is_integral_v<Y> >* = nullptr>
        Edge2D(std::pair<X, Y> vertices, enum EdgeState state = EdgeState::Unsearched) noexcept
            : from(static_cast<value_type>(vertices.first))
            , to(static_cast<value_type>(vertices.second))
            , state(_state) {}
        
    };
}