#include<iostream>
#include<bits/stdc++.h>
using namespace std;
vector<vector<vector<vector<int>>>> inputTensor()
{
    int batch, channel, height, width;
    cout<<"input the size of tensor"<<endl;
    cin>>batch>>channel>>height>>width;
    vector<vector<vector<vector<int>>>> tensor;
    tensor.resize(batch);
    for(int b=0;b<batch;b++){
        tensor[b].resize(channel);
        for(int c=0;c<channel;c++){
            tensor[b][c].resize(height);
            for(int h=0;h<height;h++){
                tensor[b][c][h].resize(width);
            }
        }
    }
    for(int b=0;b<batch;b++){
        for(int c=0;c<channel;c++){
            for(int h=0;h<height;h++){
                for(int w=0;w<width;w++){
                cin>>tensor[b][c][h][w];
                }
            }
        }
    }
    return tensor;
}
vector<vector<vector<vector<int>>>> randomTensor()
{
    int batch, channel, height, width;
    cout<<"input the size of tensor"<<endl;
    cin>>batch>>channel>>height>>width;
    vector<vector<vector<vector<int>>>> tensor;
    tensor.resize(batch);
    for(int b=0;b<batch;b++){
        tensor[b].resize(channel);
        for(int c=0;c<channel;c++){
            tensor[b][c].resize(height);
            for(int h=0;h<height;h++){
                tensor[b][c][h].resize(width);
            }
        }
    }
    for(int b=0;b<batch;b++){
        for(int c=0;c<channel;c++){
            for(int h=0;h<height;h++){
                for(int w=0;w<width;w++){
                tensor[b][c][h][w] = rand();
                }
            }
        }
    }
    return tensor;
}
void printTensor(vector<vector<vector<vector<int>>>> tensor){
    for(int b=0;b<tensor.size();b++){
        for(int c=0;c<tensor[b].size();c++){
            for(int h=0;h<tensor[b][c].size();h++){
                for(int w=0;w<tensor[b][c][h].size();w++){
                cout<<tensor[b][c][h][w]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
        cout<<endl;
    }
}
void printSize(vector<vector<vector<vector<int>>>> tensor){
    int batch = tensor.size(); 
    int channel = tensor[0].size();
    int height= tensor[0][0].size(); 
    int width = tensor[0][0][0].size();
    cout<<batch<<" "<<channel<<" "<<height<<" "<<width<<endl;
}
vector<vector<vector<vector<int>>>> maxpooling(vector<vector<vector<vector<int>>>> tensor){
    int batch = tensor.size(); 
    int channel = tensor[0].size();
    int height= tensor[0][0].size(); 
    int width = tensor[0][0][0].size();
    int out_h = (height-3+2)/2, out_w = (width-3+2)/2;
    int mod_h = (height-3+2)%2, mod_w =(width-3+2)%2;
    if (mod_h != 0) out_h++;
    if (mod_w != 0) out_w++;
    vector<vector<vector<vector<int>>>> pool_out;
    pool_out.resize(batch);
    #pragma omp parallel for
    for(int b=0;b<batch;b++){
        pool_out[b].resize(channel);
        for(int c=0;c<channel;c++){
            pool_out[b][c].resize(out_h);
            for(int i=0;i<out_h;i++){
                pool_out[b][c][i].resize(out_w);
            }
        }
    }   
    #pragma omp  parallel for
    for(int b=0;b<batch;b++){
        for(int c=0;c<channel;c++){
            for(int i=0;i<out_h;i++){
                for (int j=0;j<out_w;j++)
                {
                    int max_ele=INT_LEAST32_MIN;
                    for(int ii=0;ii<3;ii++)
                        for (int jj=0;jj<3;jj++)
                        {
                            int tmp_x=i*2+ii;
                            int tmp_y=j*2+jj;
                            if(tmp_x==0 || tmp_x==height+1 || tmp_y==0 || tmp_y==width+1){continue;}
                            if(tensor[b][c][tmp_x-1][tmp_y-1]>max_ele){
                                max_ele=tensor[b][c][tmp_x-1][tmp_y-1];
                            }
                            
                        }
                    pool_out[b][c][i][j]=max_ele;
                }
            }    
        }
    }
    return pool_out;
}
vector<vector<vector<vector<int>>>> add(vector<vector<vector<vector<int>>>> tensor1, vector<vector<vector<vector<int>>>> tensor2){
    int batch_1 = tensor1.size(); int channel_1 = tensor1[0].size();
    int height_1=tensor1[0][0].size(); int width_1=tensor1[0][0][0].size();
    int batch_2 = tensor2.size(); int channel_2 = tensor2[0].size();
    int height_2=tensor2[0][0].size(); int width_2=tensor2[0][0][0].size();
    int batch_3=max(batch_1,batch_2);
    int channel_3=max(channel_1,channel_2);
    int height_3=max(height_1,height_2);
    int width_3=max(width_1,width_2);
    vector<vector<vector<vector<int>>>> add_res (batch_3,vector<vector<vector<int>>>(channel_3,vector<vector<int>>(height_3,vector<int>(width_3,0))));
    if(batch_1!=batch_2 && batch_1!=1 && batch_2!=1){
        cout<<"batch不⼀致且无法广播"<<endl;
        return add_res;
    }
    if(channel_1!=channel_2 && channel_1!=1 && channel_2!=1){
        cout<<"channel不⼀致且无法广播"<<endl;
        return add_res;
    }
    if(height_1!=height_2 && height_1!=1 && height_2!=1){
        cout<<"height不⼀致且无法广播"<<endl;
        return add_res;
    }
    if(width_1!=width_2 && width_1!=1 && width_2!=1){
        cout<<"width不⼀致且无法广播"<<endl;
        return add_res;
    }

    #pragma omp parallel num_threads(4)
    {
        #pragma omp for
        for(int b=0;b<batch_3;b++){
            // min用于广播操作
            int b1=min(b,batch_1-1);
            int b2=min(b,batch_2-1);
            for(int c=0;c<channel_3;c++){
                int c1=min(c,channel_1-1);
                int c2=min(c,channel_2-1);
                for(int h=0;h<height_3;h++){
                    int h1=min(h,height_1-1);
                    int h2=min(h,height_2-1);
                    if(width_1==1 && width_2==1){
                        add_res[b][c][h][0]=tensor1[b1][c1][h1][0]+tensor2[b2][c2][h2][0];

                    }else{
                        int w_int=width_3/4;
                        int w_res=width_3%4;
                        for(int w=0;w<w_int;w++){
                            __m128i add;
                            if(width_1==1){
                                __m128i xx1 = _mm_setr_epi32(tensor1[b1][c1][h1][0], tensor1[b1][c1][h1][0], tensor1[b1][c1][h1][0], tensor1[b1][c1][h1][0]);
                                __m128i xx2 = _mm_setr_epi32(tensor2[b2][c2][h2][0+4*w], tensor2[b2][c2][h2][1+4*w], tensor2[b2][c2][h2][2+4*w], tensor2[b2][c2][h2][3+4*w]);
                                add = _mm_add_epi32(xx1, xx2);
                                _mm_storeu_si128((__m128i*)(&add_res[b][c][h][w*4]), add);
                            }else if(width_2==1){
                                __m128i xx1 = _mm_setr_epi32(tensor1[b1][c1][h1][0+4*w], tensor1[b1][c1][h1][1+4*w], tensor1[b1][c1][h1][2+4*w], tensor1[b1][c1][h1][3+4*w]);
                                __m128i xx2 = _mm_setr_epi32(tensor2[b2][c2][h2][0], tensor2[b2][c2][h2][0], tensor2[b2][c2][h2][0], tensor2[b2][c2][h2][0]);
                                add = _mm_add_epi32(xx1, xx2);
                                _mm_storeu_si128((__m128i*)(&add_res[b][c][h][w*4]), add);
                            }else{
                                __m128i xx1 = _mm_setr_epi32(tensor1[b1][c1][h1][0+4*w], tensor1[b1][c1][h1][1+4*w], tensor1[b1][c1][h1][2+4*w], tensor1[b1][c1][h1][3+4*w]);
                                __m128i xx2 = _mm_setr_epi32(tensor2[b2][c2][h2][0+4*w], tensor2[b2][c2][h2][1+4*w], tensor2[b2][c2][h2][2+4*w], tensor2[b2][c2][h2][3+4*w]);
                                add = _mm_add_epi32(xx1, xx2);
                                _mm_storeu_si128((__m128i*)(&add_res[b][c][h][w*4]), add);
                            }    
                        }
                        if(w_res>0){
                            for(int w_r=0;w_r<w_res;w_r++){
                                int w1=w_int*4+w_r;
                                int w2=w_int*4+w_r;
                                if(width_1==1){
                                    add_res[b][c][h][w_int*4+w_r]=tensor1[b1][c1][h1][0]+tensor2[b2][c2][h2][w2];
                                }else if(width_2==1){
                                    add_res[b][c][h][w_int*4+w_r]=tensor1[b1][c1][h1][w1]+tensor2[b2][c2][h2][0];
                                }else{
                                    add_res[b][c][h][w_int*4+w_r]=tensor1[b1][c1][h1][w1]+tensor2[b2][c2][h2][w2];
                                }
                                
                            }
                        }
                    }
                }
            }
        }
    }
    return add_res;
}
int main(){
    // vector<vector<vector<vector<int>>>> tensor1 = inputTensor();
    // vector<vector<vector<vector<int>>>> tensor2 = inputTensor();
    vector<vector<vector<vector<int>>>> tensor1 = randomTensor();
    vector<vector<vector<vector<int>>>> tensor2 = randomTensor();
    long start, end;
    start = clock();
    vector<vector<vector<vector<int>>>> tensor3 = maxpooling(tensor1);
    end = clock();
    cout<<"maxpooling time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
    start = clock();
    vector<vector<vector<vector<int>>>> tensor4 = add(tensor2, tensor3);
    end = clock();
    cout<<"element-wise add time = "<<double(end-start)/CLOCKS_PER_SEC<<"s"<<endl;
    // printTensor(tensor3);
    // printTensor(tensor4);
    printSize(tensor4);
}