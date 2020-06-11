#pragma once
# include<Siv3D.hpp>
using namespace s3d;

# include<Edge2D.hpp>
# include<Vertex2D.hpp>



namespace ugis
{	
	/// <summary>
    /// ２次元グラフ
    /// </summary>
	class Graph
	{
	private:
		using ListType = std::pair<size_t, int32>;

		Font m_font = Font(16);
		Array<ColorF> m_vertexColor = defaultColor();
		Array<ColorF> m_edgeColor = defaultColor();
		double m_radius = 10.0;
		double m_thickness = 3.0;
		size_t m_height = 600;
		size_t m_width = 800;
		int32 m_grab = -1;
		Rect m_viewportRect = Rect(0, 0, 120, 600);
		size_t m_begin = 0;
	public:
		Array<ugis::Vertex2D> vertex;
		Array<Edge2D> edges;
		Array<Array<ListType>> connection;
		Array<int32> dist;
		const int32 INF = static_cast<int32>(1e9);

		static Array<ColorF> defaultColor() 
		{
			Array<ColorF> colors;
			colors << ColorF(U"#818181");
			colors << ColorF(U"#B0A000");
			colors << ColorF(U"#ECDC00");
			return colors;
		};
		static Font defaultFont() 
		{
			return Font(16);
		}

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		Graph() = default;
		constexpr Graph(const Graph&) = default;

		/// <summary>
		/// 隣接リスト(辺の重み付き)からグラフを作成します。
		/// </summary>
		/// <param name="connection">
		/// グラフを表す隣接リスト
		/// </param>
		template<class X, class W, std::enable_if_t< std::is_integral_v<X> && std::is_arithmetic_v<W> >* = nullptr>
		constexpr Graph(Array<Array<std::pair<X, W>>>& _connection)
			: connection(static_cast<Array<Array<ListType>>>(_connection)){ init(); }
		
		/// <summary>
		/// 隣接リストからグラフを作成します。
		/// </summary>
		/// <param name="connection">
		/// グラフを表す隣接リスト
		/// </param>
		template<class X, std::enable_if_t<std::is_integral_v<X>>* = nullptr>
		constexpr Graph(Array<Array<X>>& _connection)
		{ 	
			Array<Array<ListType>> newConnection;
			for(const Array<X>& children : _connection)
			{
				Array<ListType> tmp;
				for(const X& child : children)
				{
					tmp << ListType(child, 1);
				}
				newConnection << tmp;
			}
			connection = newConnection;
			init(); 
		}

		/// <summary>
		/// 隣接リストを返す関数からグラフを作成します。
		/// </summary>
		/// <param name="generater">
		/// 隣接リストを返す関数
		/// </param>
		template <class Fty, std::enable_if_t<std::is_function_v<Fty>>* = nullptr>
		constexpr Graph(Fty generater)
			: connection(generater()) { init(); }
		
		
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
		/// <param name="font">
		/// 頂点番号、辺の重みなどのフォント
		/// </param>
		void init(size_t width = 800, size_t height = 600, double radius = 10.0, double thickness = 3.0
		, Array<ColorF> vertexColor = defaultColor(), Array<ColorF> edgeColor = defaultColor(), Font font = defaultFont());

		/// <summary>
		/// グラフの描画します。
		/// </summary>
		/// <param name="update">
		/// draw関数内で、System::Update()をするかどうか
		/// </param>
		/// <param name="delay">
		/// 描画を遅延する時間
		/// (update==trueの時しか使用されません)
		/// </param>
		/// <return>
		/// 描画の成功/失敗
		/// </return>
		bool draw(bool update = false,bool displayEdgeWeight = true, double delay = 0.5);

		///
		/// ここにドキュメントをかく
		/// 
		void load(String path);

		/*
		TODO:	loadをコンストラクタでできるように
				頂点vまでの最短距離の表をつくる

		*/
	};
}