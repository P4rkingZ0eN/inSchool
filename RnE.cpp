#include <bits/stdc++.h>
#pragma GCC optimize("Ofast")
using namespace std;
using lf=double;
const lf k1=10,k2=0.005;
vector<vector<lf> >dist;
vector<set<int> >V;
vector<vector<int> >G;
set<int>A;
int N;
lf D[(1<<21)][23];
lf f(lf r){return k1/(1+exp(k2*r));}
lf C(int u){
    lf ret=0;
    for(register int v=1;v<=N;v++)if(v^u)ret+=(dist[u][v]);
    return ret;
}
void Divde(){
    if(A.empty())return;
    lf mx=0,X=0;
    register int i;
    for(auto it=A.begin();it!=A.end();it++){
        lf now=C(*it);
        if(now>mx){mx=now;X=*it;}
    }
    vector<pair<lf,int> >B;
    for(auto it=A.begin();it!=A.end();it++)B.push_back({dist[X][*it],*it});
    V.push_back(set<int>());
    sort(B.begin(),B.end());
    for(i=0;i<(int)B.size();i++){
        if(A.empty())break;
        if(V.back().empty()){
            A.erase(B[i].second);
            V.back().insert(B[i].second);
            continue;
        }
        lf M=f(0);bool flag=true;
        for(auto it=V.back().begin();it!=V.back().end();it++){
            flag&=(0.85*M<=f(dist[B[i].second][*it]));
            //cout<<M<<" "<<f(dist[B[i].second][*it])<<endl;
        }
        if(flag){
            A.erase(B[i].second);
            V.back().insert(B[i].second);
        }
    }
    Divde();
    return;
}
lf TSP(int bit,int cur){
    if(bit==(1<<N)-1)return dist[cur][1];
    if(D[bit][cur])return D[bit][cur];
    D[bit][cur]=9e18;
    for(int x:G[cur]){
        if(bit&(1<<(x-1)))continue;
        D[bit][cur]=min(D[bit][cur],TSP(bit|(1<<(x-1)),x)+dist[cur][x]);
    }
    return D[bit][cur];
}
lf opt_TSP(int bit,int cur){
    if(bit==(1<<N)-1)return dist[cur][1];
    if(D[bit][cur])return D[bit][cur];
    D[bit][cur]=9e18;
    for(int x=1;x<=N;x++){
        if(bit&(1<<(x-1)))continue;
        D[bit][cur]=min(D[bit][cur],opt_TSP(bit|(1<<(x-1)),x)+dist[cur][x]);
    }
    return D[bit][cur];
}
int main(){
    freopen("8.in","r",stdin);
    freopen("out8____.txt","w",stdout);
    ios_base::sync_with_stdio(false);cin.tie(NULL);
    cin>>N;
    register int i,j;
    dist.resize(N+1);
    G.resize(N+1);
    double mn=1e18;
    for(i=1;i<=N;i++){
        A.insert(i);
        dist[i].resize(N+1);
        for(j=1;j<=N;j++){
            lf a;cin>>a;
            dist[i][j]=a;
            if(a>0)mn=min(a,mn);
        }
    }
    for(int i=1;i<=N;i++)for(int j=1;j<N;j++)if(dist[i][j]>0)dist[i][j]-=mn;
    int SS=clock();
    Divde();
    int EE=clock();
    cout<<"Number of Set: "<<V.size()<<"\n\n";
    cout<<"k1 and k2\nk1: "<<k1<<"   k2: "<<k2<<"\n\n";
    cout<<"____SET_info______\n";
    for(i=0;i<(int)V.size();i++){
        cout<<"["<<i+1<<"] ";
        for(auto it=V[i].begin();it!=V[i].end();it++)cout<<*it<<" ";
        cout<<"\n";
    }
    cout<<"Running Time: "<<EE-SS<<"ms\n\n\n";
    for(i=0;i<(int)V.size();i++){
        for(auto it=V[i].begin();it!=V[i].end();it++){
            for(auto jt=V[i].begin();jt!=V[i].end();jt++){
                if(*it==*jt)continue;
                G[*it].push_back(*jt);
                G[*jt].push_back(*it);
            }
        }
    }
    for(i=0;i<(int)V.size();i++){
        for(j=i+1;j<(int)V.size();j++){
            lf mn=9e18;int p=0,q=0;
            for(auto it=V[i].begin();it!=V[i].end();it++){
                for(auto jt=V[j].begin();jt!=V[j].end();jt++){
                    if(dist[*it][*jt]<mn){mn=dist[*it][*jt];p=*it;q=*jt;}
                }
            }
            G[p].push_back(q);
            G[q].push_back(p);
        }
    }
    cout<<"RESULT\n";
    SS=clock();
    cout<<"Weight of Tour: "<<TSP(1,1)<<"\n";
    EE=clock();
    cout<<"Running Time: "<<EE-SS<<"ms\n\n";
    SS=clock();
    memset(D,0,sizeof(D));
    cout<<"Weight of Optimal Tour: "<<opt_TSP(1,1)<<"\n";
    EE=clock();
    cout<<"Running Time: "<<EE-SS<<"ms\n\n";
    return 0;
}
