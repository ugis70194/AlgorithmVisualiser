# pragma once
# ifndef ugis_EDGE2D_INCLUDED
    # define ugis_EDGE2D_INCLUDED
# endif

# include <State.hpp>

// edgeを描くには2つの頂点の位置が必要
// edge class objectに頂点の位置を持たせるのはメモリの無駄
// vertexはどうせArray<vertex>で持つ(Graphを作るためのだけのクラスだし)
// それならindex = vertex_idでよくない？

namespace ugis
{       


    /// <summary>
    /// ２次元グラフ上の辺
    /// </summary>
    struct Edge2D 
    {   
        using value_type = size_t;
        using weight_type = int32;

        value_type from; 
        value_type to;
        weight_type weight;  
        State state; 

        /// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
        Edge2D() noexcept = default;
        constexpr Edge2D(const Edge2D&) noexcept = default; 

        constexpr Edge2D(const value_type _from,const value_type _to,const weight_type _weight = (weight_type)1,const State _state = State::Unsearched) noexcept
            : from(_from)
            , to(_to)
            , weight(_weight)
            , state(_state) { if(from > to) std::swap(from, to); }

        template<class X, class Y, class W, 
        std::enable_if_t< std::is_integral_v<X> && std::is_integral_v<Y> && std::is_arithmetic_v<W>>* = nullptr>
        constexpr Edge2D(const X _from,const Y _to,const W _weight = (weight_type)1,const State _state = State::Unsearched) noexcept
            : from(static_cast<value_type>(_from))
            , to(static_cast<value_type>(_to))
            , weight(static_cast<weight_type>(_weight))
            , state(_state) { if(from > to) std::swap(from, to); }

        template<class X, class Y, class W, 
        std::enable_if_t< std::is_integral_v<X> && std::is_integral_v<Y> && std::is_arithmetic_v<W>>* = nullptr>
        Edge2D(const X _from,const std::pair<Y,W> toAndWeight,const State _state = State::Unsearched) noexcept
            : from(static_cast<value_type>(_from))
            , to(static_cast<value_type>(toAndWeight.first))
            , weight(static_cast<weight_type>(toAndWeight.second))
            , state(_state) { if(from > to) std::swap(from, to); }
    
        template<class X, class Y, class W, 
        std::enable_if_t< std::is_integral_v<X> && std::is_integral_v<Y> && std::is_arithmetic_v<W>>* = nullptr>
        Edge2D(const std::pair<X, Y>& vertices,const W& _weight,const State _state = State::Unsearched) noexcept
            : from(static_cast<value_type>(vertices.first))
            , to(static_cast<value_type>(vertices.second))
            , weight(static_cast<weight_type>(_weight))
            , state(_state) { if(from > to) std::swap(from, to); }

        inline const bool operator==(const Edge2D& e) const 
        {
            return (e.from == from && e.to == to);
        }
    };
}