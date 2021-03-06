#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

namespace NNetwork {
    typedef long long TFlow;
    typedef size_t TVertex;
    typedef size_t TVertexNumber;

    struct Path {
        TFlow flow = 0;
        vector<TVertex> path;
    };


    struct Edge {
        TVertex from;
        TVertex to;
        TFlow capacity;
        TFlow flow;
        TVertexNumber number;
    };

    class Network {
     private:
        vector<Edge> edges_;
        vector<Edge> rev_edges_;
        vector<int> lasts_;
        vector<int> rev_lasts_;
        vector<int> prevEdge_;
        vector<int> rev_prevEdge_;
        vector<int> nextEdge_;
        vector<int> rev_nextEdge_;
        vector<TVertex> vertexInSInMinCut_;
        vector<Path> decomposition_;
        vector<TFlow> circle_;
        vector<TFlow> lowCapacity_;

        TVertexNumber vertexNumber_;
        TVertex source_;
        TVertex sink_;
        long long scale_;

        void addStraightEdgeLocal_(TVertex start, TVertex finish,
                                   TFlow capacity, TVertexNumber number,
                                   vector<Edge> &edges,
                                   vector<int> &nextEdge,
                                   vector<int> &prevEdge,
                                   vector<int> &lasts) {
            edges.emplace_back(Edge{start, finish, capacity,
                                    static_cast<TFlow>(0), number});
            nextEdge.push_back(lasts[start]);
            prevEdge.push_back(-1);
            if (lasts[start] != -1)
                prevEdge[lasts[start]] = edges.size() - 1;
            lasts[start] = edges.size() - 1;
        }

        void addEdgeLocal_(TVertex start, TVertex finish, TFlow capacity, TVertexNumber number) {
            addStraightEdgeLocal_(start, finish, capacity, number,
                                  edges_, nextEdge_,
                                  prevEdge_, lasts_);
            addStraightEdgeLocal_(finish, start, capacity, number,
                                  rev_edges_, rev_nextEdge_,
                                  rev_prevEdge_, rev_lasts_);
        }

        void pushFlow_(int edge, TFlow flow) {
            edges_[edge].flow += flow;
            edges_[edge ^ 1].flow -= flow;
            rev_edges_[edge].flow += flow;
            rev_edges_[edge ^ 1].flow -= flow;
        }

     public:
        static const TVertex INF_VERTEX = INT32_MAX;
        static const TFlow INF_FLOW = 1e15;

        Network(TVertexNumber n, TVertex s, TVertex t)
                : edges_(0),
                  rev_edges_(0),
                  lasts_(n, -1),
                  rev_lasts_(n, -1),
                  prevEdge_(0),
                  rev_prevEdge_(0),
                  nextEdge_(0),
                  rev_nextEdge_(0),
                  decomposition_(0),
                  circle_(0),
                  lowCapacity_(n),
                  vertexNumber_(n),
                  source_(s),
                  sink_(t),
                  scale_((1LL << 32LL)) {
            if (s >= n || t >= n)
                cerr << "s >= n || t >= n";
            if (s == t)
                cerr << "s == t";
        }

        void addEdge(TVertex start, TVertex finish, TFlow capacity, TVertexNumber number) {
            if (capacity < static_cast<TFlow>(0))
                cerr << "capacity < static_cast<TFlow>(0)";
            if (start == finish)
                return;
            addEdgeLocal_(start, finish, capacity, number);
            addEdgeLocal_(finish, start, static_cast<TFlow>(0), INF_VERTEX);
            // lowCapacity_[number] = capacity;
        }

        class EdgeIterator {
         private:
            friend class Network;

            int edgeId_;
            Network &myNetwork_;

            void goNext_() {
                if (!isValid())
                    cerr << "goNext_ is bad iterator";
                edgeId_ = myNetwork_.nextEdge_[edgeId_];
            }

            void goRevNext_() {
                if (!isValid())
                    cerr << "goRevNext_ is bad iterator";
                edgeId_ = myNetwork_.rev_nextEdge_[edgeId_];
            }

         public:
            EdgeIterator(int edgeId, Network &net)
                    : edgeId_(edgeId),
                      myNetwork_(net) {}

            int getEdgeId() {
                return edgeId_;
            }

            bool isValid() const {
                return edgeId_ >= 0;
            }

            EdgeIterator &goNext(size_t t = 1) {
                for (size_t i = 0; i < t; ++i) {
                    goNext_();
                }
                return *this;
            }

            EdgeIterator &goRevNext(size_t t = 1) {
                for (size_t i = 0; i < t; ++i) {
                    goRevNext_();
                }
                return *this;
            }

            TVertex getStart() const {
                return myNetwork_.edges_[edgeId_].from;
            }

            TVertex getRevStart() const {
                return myNetwork_.rev_edges_[edgeId_].from;
            }

            TVertex getFinish() const {
                return myNetwork_.edges_[edgeId_].to;
            }

            TVertex getRevFinish() const {
                return myNetwork_.rev_edges_[edgeId_].to;
            }

            TFlow getCapacity() const {
                return myNetwork_.edges_[edgeId_].capacity;
            }

            TFlow getRevCapacity() const {
                return myNetwork_.rev_edges_[edgeId_].capacity;
            }

            TFlow getFlow() const {
                return myNetwork_.edges_[edgeId_].flow;
            }

            TFlow getRevFlow() const {
                return myNetwork_.rev_edges_[edgeId_].flow;
            }

            TVertexNumber getNumber() const {
                return myNetwork_.edges_[edgeId_].number;
            }

            TVertexNumber getLowCapacity() const {
                return (size_t) myNetwork_.lowCapacity_[(size_t) getNumber()];
            }

            TFlow getResidualCapacity() const {
                if (getCapacity() - getFlow() < 0)
                    cerr << "getCapacity() - getFlow() < 0";
                return getCapacity() - getFlow();
            }

            TFlow getRevResidualCapacity() const {
                return getRevCapacity() - getRevFlow();
            }

            void pushFlow(TFlow f) const {
                myNetwork_.pushFlow_(edgeId_, f);
            }


            void deleteEdge() {
                if (myNetwork_.nextEdge_[edgeId_] != -1)
                    myNetwork_.prevEdge_[myNetwork_.nextEdge_[edgeId_]]
                            = myNetwork_.prevEdge_[edgeId_];
                if (myNetwork_.prevEdge_[edgeId_] != -1) {
                    myNetwork_.nextEdge_[myNetwork_.prevEdge_[edgeId_]]
                            = myNetwork_.nextEdge_[edgeId_];
                } else {
                    myNetwork_.lasts_[myNetwork_.edges_[edgeId_].from]
                            = myNetwork_.nextEdge_[edgeId_];
                }
            }

            void deleteRevEdge() {
                if (myNetwork_.rev_nextEdge_[edgeId_] != -1)
                    myNetwork_.rev_prevEdge_[myNetwork_.rev_nextEdge_[edgeId_]]
                            = myNetwork_.rev_prevEdge_[edgeId_];
                if (myNetwork_.rev_prevEdge_[edgeId_] != -1) {
                    myNetwork_.rev_nextEdge_[myNetwork_.rev_prevEdge_[edgeId_]]
                            = myNetwork_.rev_nextEdge_[edgeId_];
                } else {
                    myNetwork_.rev_lasts_[myNetwork_.rev_edges_[edgeId_].from]
                            = myNetwork_.rev_nextEdge_[edgeId_];
                }
            }
        };

        EdgeIterator getListBegin(TVertex v) {
            return EdgeIterator(lasts_[v], *this);
        }

        EdgeIterator getRevListBegin(TVertex v) {
            return EdgeIterator(rev_lasts_[v], *this);
        }

        const vector<Edge> &getEdges() const {
            return edges_;
        }

        const vector<Edge> &getRevEdges() const {
            return rev_edges_;
        }

        const vector<int> &getLasts() const {
            return lasts_;
        }

        const vector<int> &getNextEdge() const {
            return nextEdge_;
        }

        const vector<int> &getRevLasts() const {
            return rev_lasts_;
        }

        const vector<int> &getRevNextEdge() const {
            return rev_nextEdge_;
        }

        const vector<int> &getPrevEdge() const {
            return prevEdge_;
        }

        const vector<int> &getRevPrevEdge() const {
            return rev_prevEdge_;
        }

        TVertexNumber getVertexNumber() const {
            return vertexNumber_;
        }

        TVertex getSource() const {
            return source_;
        }

        TVertex getSink() const {
            return sink_;
        }

        long long getScale() const {
            return scale_;
        }

        void setScale(long long scale) {
            scale_ = scale;
        }

        const vector<TVertex> &getVertexInSInMinCut() const {
            return vertexInSInMinCut_;
        }

        void setVertexInSInMinCut(const vector<TVertex> &vertexInSInMinCut) {
            vertexInSInMinCut_ = vertexInSInMinCut;
        }

        const vector<Path> &getDecomposition() const {
            return decomposition_;
        }

        void setDecomposition(const vector<Path> &decomposition) {
            decomposition_ = decomposition;
        }

        const vector<TFlow> &getCircle() const {
            return circle_;
        }

        void setCircle(const vector<TFlow> &circle) {
            circle_ = circle;
        }

        void subtractFlowAtPos(const size_t &position, const long long &value) {
            edges_[position].flow -= value;
        }

        void cleanNet() {
            lasts_.assign(vertexNumber_, -1);
            rev_lasts_.assign(vertexNumber_, -1);
            edges_.resize(0);
            rev_edges_.resize(0);
            nextEdge_.resize(0);
            rev_nextEdge_.resize(0);
        }
    };
}

NNetwork::TFlow getFlowValue(NNetwork::Network &network) {
    NNetwork::TFlow ans(0);
    for (auto it = network.getListBegin(network.getSource()); it.isValid(); it.goNext())
        ans += it.getFlow();
    return ans;
}

namespace NMaxFlow {
    class MaxFlowFinder {
     protected:
        NNetwork::Network *net_;
     public:
        MaxFlowFinder() : net_(nullptr) {}

        ~MaxFlowFinder() = default;

        void setNetwork(NNetwork::Network *network) {
            net_ = network;
        }

        virtual void buildMaxFlow() = 0;
    };

    class BlockingFlowBase : public MaxFlowFinder {
     protected:
        vector<long long> dist_;

        virtual void init_() = 0;

        bool bfs_() {
            queue<NNetwork::TVertex> q;
            dist_.assign(net_->getVertexNumber(), NNetwork::Network::INF_VERTEX);
            dist_[net_->getSource()] = 0;
            q.push(net_->getSource());
            while (!q.empty()) {
                NNetwork::TVertex v = q.front();
                q.pop();
                for (auto it = net_->getListBegin(v); it.isValid(); it.goNext()) {
                    if (it.getResidualCapacity() < net_->getScale())
                        continue;
                    if (dist_[it.getFinish()] != NNetwork::Network::INF_VERTEX)
                        continue;
                    dist_[it.getFinish()] = dist_[v] + 1;
                    q.push(it.getFinish());
                }
            }
            return dist_[net_->getSink()] != NNetwork::Network::INF_VERTEX;
        }

        vector<NNetwork::TVertex> vertexInSInMinCut() {
            vector<NNetwork::TVertex> ans;
            for (size_t i = 0; i < net_->getVertexNumber(); ++i)
                if (dist_[i] != NNetwork::Network::INF_VERTEX)
                    ans.push_back(i + 1);
            return ans;
        }

        NNetwork::Path findPath() {
            NNetwork::Path ans = NNetwork::Path();
            deque<pair<NNetwork::Edge, size_t>> Q;
            vector<bool> used(net_->getVertexNumber(), false);
            NNetwork::TVertex v = net_->getSource();
            while (!used[v]) {
                if (v == net_->getSink())
                    break;
                used[v] = true;
                for (auto it = net_->getListBegin(v); it.isValid(); it.goNext()) {
                    if (it.getNumber() != NNetwork::Network::INF_VERTEX &&
                        !used[it.getFinish()] && it.getFlow() > 0) {
                        Q.emplace_back(net_->getEdges()[it.getEdgeId()], it.getEdgeId());
                        v = it.getFinish();
                        break;
                    }
                }
            }

            NNetwork::TFlow min_flow = NNetwork::Network::INF_FLOW;
            for (auto curr_edge : Q) {
                min_flow = min(min_flow, curr_edge.first.flow);
                ans.path.push_back(curr_edge.first.number);
            }
            ans.flow = min_flow;
            for (auto curr_edge : Q) {
                net_->subtractFlowAtPos(curr_edge.second, min_flow);
            }
            return ans;
        }

        void buildDecomposition() {
            vector<NNetwork::Path> ans;
            NNetwork::Path curr_path = findPath();
            while (!curr_path.path.empty()) {
                ans.push_back(curr_path);
                curr_path = findPath();
            }
            reverse(ans.begin(), ans.end());
            net_->setDecomposition(ans);
        }

        virtual void findBlockingFlow_() = 0;

     public:
        void buildMaxFlow() final {
            init_();
            while (net_->getScale() > 0) {
                while (bfs_())
                    findBlockingFlow_();
                net_->setScale(net_->getScale() / 2);
            }
            // net_->setVertexInSInMinCut(vertexInSInMinCut());
            buildDecomposition();
        }

        bool findCircle() {
            vector<NNetwork::TFlow> ans(net_->getVertexNumber(), 0);
            for (auto it = net_->getListBegin(0); it.isValid(); it.goNext()) {
                if (it.getResidualCapacity() != 0) {
                    return false;
                }
            }
            for (size_t i = 1; i < net_->getVertexNumber() - 1; ++i) {
                for (auto it = net_->getListBegin(i); it.isValid(); it.goNext()) {
                    if (it.getNumber() != NNetwork::Network::INF_VERTEX
                        && it.getFinish() != net_->getVertexNumber() - 1) {
                        ans[it.getNumber()] = it.getFlow() + it.getLowCapacity();
                    }
                }
            }
            net_->setCircle(ans);
            return true;
        }
    };

    class DinicAlgorithm : public BlockingFlowBase {
     private:
        vector<int> start;

        void init_() final {
            start.resize(net_->getVertexNumber());
        }

        NNetwork::TFlow sendFlow(NNetwork::TVertex u, NNetwork::TFlow flow) {
            if (u == net_->getSink())
                return flow;

            for (NNetwork::Network::EdgeIterator it(start[u], *net_);
                 it.isValid();
                 it.goNext(), start[u] = it.getEdgeId()) {
                if (dist_[it.getFinish()] == dist_[it.getStart()] + 1 &&
                    it.getResidualCapacity() >= net_->getScale()) {
                    NNetwork::TFlow curr_flow = min(flow, it.getResidualCapacity());
                    NNetwork::TFlow temp_flow = sendFlow(it.getFinish(), curr_flow);
                    if (temp_flow > 0) {
                        it.pushFlow(temp_flow);
                        return temp_flow;
                    }
                }
            }

            return 0;
        }

        void findBlockingFlow_() final {
            for (size_t i = 0; i < net_->getVertexNumber(); ++i) {
                auto it = net_->getListBegin(i);
                start[i] = it.getEdgeId();
            }
            while (sendFlow(net_->getSource(), NNetwork::Network::INF_FLOW)) {}
        }
    };
}

// const string FILE_NAME = "chvatal";
int main() {
    // freopen((FILE_NAME + ".in").c_str(), "r", stdin);
    // freopen((FILE_NAME + ".out").c_str(), "w", stdout);
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    size_t n, m;
    cin >> n >> m;
    auto *net = new NNetwork::Network(n, 0, n - 1);
    long long max_capacity = -1;
    for (size_t i = 0; i < m; ++i) {
        size_t a, b;
        long long capacity;
        cin >> a >> b >> capacity;
        max_capacity = max(max_capacity, capacity);
        net->addEdge(a - 1, b - 1, capacity, i);
    }
    net->setScale(1LL << ((long long) log2((double) max_capacity)));
    auto solver = NMaxFlow::DinicAlgorithm();
    solver.setNetwork(net);
    solver.buildMaxFlow();
    cout << net->getDecomposition().size() << '\n';
    for (auto &curr_path : net->getDecomposition()) {
        cout << curr_path.flow << ' ' << curr_path.path.size() << ' ';
        for (const auto &v : curr_path.path)
            cout << v + 1 << ' ';
        cout << '\n';
    }
    return 0;
}

