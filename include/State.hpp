#pragma once
# ifndef ugis_STATE_INCLUDED
    # define ugis_STATE_INCLUDED
# endif

namespace ugis
{   
    
    /// <summary>
    /// 辺の状態を表す
    /// </summary>
    enum class State
    {
        Unsearched = 0,
        Searched = 1,
        Confirmed = 2
    };
}