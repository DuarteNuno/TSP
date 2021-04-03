#include "util.h"


bool accept(int actual_p,int neighbour_p,int temp){

    double dif = neighbour_p-actual_p;
    double e = -dif/temp;
    double x = ((double) rand() / (double) (RAND_MAX));

 /*    cout << "e = " << e << endl;
    cout << "x = " << x << endl; */
    
     if(actual_p>neighbour_p){
       // cout << "hallo2" << endl;
        return true;
    }
    
    else return e > x;
}

void random_vector(Map *m){
    random_shuffle(m->Points.begin(), m->Points.end());
    vector<Point*> r = m->Points;
    r.push_back(r.front());
    m->Path = r;
}

void order(Map *m){ 
    m->Print_Points();
}

void print_Path(Map *m){ 
    for(auto const& i : m->Path){
        i->Point_Print();
    }
}

int cross_Product(int x1,int y1,int x2,int y2){
    return (x1*y2)-(x2*y1);    
}

bool on_segment(Point* p1, Point* p2, Point* p3){ //check if p3 is on p1p2 line
   
    if((min(p1->x,p2->x) <= p3->x) && 
       (p3->x <= max(p1->x,p2->x)) && 
       (min(p1->y,p2->y) <= p3->y) && 
       (p3->y <= max(p1->y,p2->y))) return true;
        
        else return false;
}

bool vectors_Intersect(Point* p1i, Point* p1f, Point* p2i, Point* p2f){
                            //reta P1i->P1f & P1i->P2i

                            //p1f-p1i // p2i-p1i
    int d1 = cross_Product((p1f->x-p1i->x),(p1f->y-p1i->y),
                             (p2i->x-p1i->x),(p2i->y-p1i->y));

                            //reta P1i->P1f & P1i->P2f
                            //cout pontos!!!!!!
    int d2 = cross_Product((p1f->x-p1i->x),(p1f->y-p1i->y),
                              (p2f->x-p1i->x),(p2f->y-p1i->y));
    
                            //reta P2i->P2f & P2i->P1i
    int d3 = cross_Product((p2f->x-p2i->x),(p2f->y-p2i->y),
                              (p1i->x-p2i->x),(p1i->y-p2i->y));

                            //reta P2i->P2f & P2i->P1f
    int d4 = cross_Product((p2f->x-p2i->x),(p2f->y-p2i->y),
                              (p1f->x-p2i->x),(p1f->y-p2i->y));

    /* cout<<d1<<endl;
    cout<<d2<<endl;
    cout<<d3<<endl;
    cout<<d4<<endl;
 */
    
    if(((d1>0 && d2<0) || (d1<0 && d2>0)) &&
    ((d3>0 && d4<0) || (d3<0 && d4>0)))
    return true;

    else if((d1==0) && 
        (on_segment(p1i,p1f,p2i)))
        return true;
    else if((d2==0) && 
        (on_segment(p1i,p1f,p2f)))
        return true;
    else if((d3==0) && 
        (on_segment(p2i,p2f,p1i)))
        return true;
    else if((d4==0) && 
        (on_segment(p2i,p2f,p1f)))
        return true;

    return false;
}

vector<vector<Point*>>* two_exchange(vector<Point*> p){
    vector<vector<Point*>>* Solution = new vector<vector<Point*>>;
    for(int i = 1; i<p.size()-2; i++){  
        for(int j = i+1; j<p.size()-1; j++){
            /*  p[i-1]->Point_Print();
             p[i]->Point_Print();
             p[j]->Point_Print();
             p[j+1]->Point_Print();
             cout << endl;
 */
            if(p.at(i-1)!=p.at(j+1)){
               /*  cout<<"hello"<< endl;
                cout << vectors_Intersect(p[i-1],p[i],p[j],p[j+1])<<endl; */
                if(vectors_Intersect(p[i-1],p[i],p[j],p[j+1])) {
                   /*  
                    cout <<"intersecao entre:\n";
                    p[i-1]->Point_Print();
                    cout <<"<->\n";
                    p[i]->Point_Print();
                    cout << "e \n";
                    p[j]->Point_Print();
                     cout <<"<->\n";
                    p[j+1]->Point_Print();
                     */
                    vector<Point*> tmp=p;
                    reverse(tmp.begin()+i+1,tmp.begin()+j);
                    Point *t = tmp[i];
                    tmp[i]=tmp[j];
                    tmp[j]=t;

                    Solution->push_back(tmp);                    
                }  
            }
        }
    }
    return Solution;
}

int n_Intersections(vector<Point*> p){
    int s=0;
    for(int i = 1; i<p.size()-2; i++){
        for(int j = i+1; j<p.size()-1; j++){
            if(p[i-1]!=p[j+1]){
                if(vectors_Intersect(p[i-1],p[i],p[j],p[j+1])) {
                    s++;
                }  
            }
        }
    }
    return s;
}

double perimeters(vector<Point*> v){
    double s=0;
    for(int i=1;i<v.size();i++){
        double x1 = v[i-1]->x;
        double x2 = v[i]->x;
        double y1 = v[i-1]->y;
        double y2 = v[i]->y;

        double line_size=sqrt(pow(x2-x1,2.0)+pow(y2-y1,2.0));
        s+=line_size;
    }

    return s;
}

vector<Point*> choose_opt(char opt, vector<vector<Point*>>* candidates){
    
    vector<Point*> n;
    
    double min_per = DBL_MAX;
    int min_intr = INT_MAX;

    switch(opt) {
        case 'a': //menor perimetro   
            for(int i=0;i<candidates->size();i++){                   
                double perimeter=perimeters(candidates->at(i));
               
                if(perimeter<=min_per){
                    min_per=perimeter;
                    n=candidates->at(i);
                }
            }
            break;
        
        case 'b': // primeiro candidato
            if(candidates->empty()){
                return vector<Point*>();
            }
            n=candidates->front();
            break;
        
        case 'c':// menos conflitos
            for(int i=0;i<candidates->size();i++){
                int min=n_Intersections(candidates->at(i));
                if(min<min_intr){
                    min_intr=min;
                    n = candidates->at(i);
                }
            }
            break;

        default:// random
            if(candidates->empty()){
                return vector<Point*>();
            }
            int r =rand()%(candidates->size());
            n=candidates->at(r);
    }
    return n;
}