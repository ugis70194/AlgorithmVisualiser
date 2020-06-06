#pragma once
# include<Siv3D.hpp>
using namespace s3d;

# include<Edge2D.hpp>
# include<Vertex2D.hpp>


namespace ugis
{	
	
	constexpr Vec2 refPoint(50, 50);
	//const size_t width(Window::ClientWidth() - 100);
	//const size_t height(Window::ClientHeight() - 100);

	/// <summary>
    /// ２次元グラフ
    /// </summary>
	class Graph
	{
	private:
		Array<ColorF> m_vertexColor = defaultColor();
		Array<ColorF> m_edgeColor = defaultColor();
		double m_radius = 10.0;
		double m_thickness = 3.0;
		size_t m_height = 600;
		size_t m_width = 800;
	public:
		Array<ugis::Vertex2D> vertex;
		Array<Edge2D> edges;
		Array<Array<size_t>> connection;

		static Array<ColorF> defaultColor() 
		{
			Array<ColorF> colors;
			colors << ColorF(U"#818181");
			colors << ColorF(U"#B0A000");
			colors << ColorF(U"#ECDC00");
			return colors;
		};

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		Graph() noexcept = default;
		constexpr Graph(const Graph&) = default;

		/// <summary>
		/// 隣接リストからグラフを作成します。
		/// </summary>
		/// <param name="connection">
		/// グラフを表す隣接リスト
		/// </param>
		template<class X, std::enable_if_t< std::is_integral_v<X>>* = nullptr>
		constexpr Graph(Array<Array<X>>& connection)
			: connection(static_cast<Array<Array<size_t>>>(connection)){ init(800, 600); }

		/// <summary>
		/// 隣接リストを返す関数からグラフを作成します。
		/// </summary>
		/// <param name="generater">
		/// 隣接リストを返す関数
		/// </param>
		template <class Fty, std::enable_if_t<std::is_function_v<Fty>>* = nullptr>
		constexpr Graph(Fty generater)
			: connection(generater()) { init(800, 600); }
		
		
		/// <summary>
		/// グラフの描画設定を初期化します。
		/// </summary>
		/// <param name="highet">
		/// 描画領域の縦幅
		/// </param>
		/// <param name="width">
		/// 描画領域の横幅
		/// </param>
		/// <param name="rudius">
		/// 辺の太さ
		/// </param>
		/// <param name="thickness">
		/// 辺の太さ
		/// </param>
		/// <param name="vertexColor">
		/// 頂点の 各状態を表す色 の配列
		/// 前から Unsearched, Searched, Confirmedの順に割り当てられます。
		/// </param>
		/// <param name="edgeColor">
		/// 辺の 各状態を表す色 の配列
		/// 前から Unsearched, Searched, Confirmedの順に割り当てられます。
		/// </param>
		void init(size_t width, size_t height, double radius = 10.0, double thickness = 3.0
		, Array<ColorF> vertexColor = defaultColor(), Array<ColorF> edgeColor = defaultColor());

		/// <summary>
		/// グラフの描画します。
		/// </summary>
		bool draw(bool update = false, double delay = 0.5);
		Array<size_t> get()
		{
			Array<size_t> tmp;
			tmp << m_width;
			tmp << m_height;
			return  tmp;
		}
	};
}