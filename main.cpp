/******************************************************************************
                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.
*******************************************************************************/

#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <iterator>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
int p;

// INPUT, OUTPUT, GATE INFO
string get_input_variable(string s){
    int i = s.find('(');
    int j = s.find(')');
    string a = s.substr((i + 1), (j - i - 1));
    return a;
}

string gate_out(string s){
    int i = s.find(' ');
    string output_of_gate = s.substr(0, i);
    return output_of_gate;
}

string gate_name(string s){
    int i = s.find(' ');
    int j = s.find('(');
    string gate = s.substr((i + 3), (j - i - 3));
    return gate;
}

string gate_input1(string s){
    string input1_of_gate;
    int j = s.find('(');
    int k = s.find(')');
    int l;
    string gate1 = gate_name(s);
    if(gate1 == "NOT"){
        input1_of_gate = s.substr((j + 1), (k - j - 1));
    }
    else{
        l = s.find(',');
        input1_of_gate = s.substr((j + 1), (l - j - 1));
    }
    return input1_of_gate;
}

string gate_input2(string s){
    string input2_of_gate;
    int j = s.find('(');
    int k = s.find(')');
    int l;
    string gate2 = gate_name(s);
    if(gate2 == "NOT"){
        input2_of_gate = "";
    }
    else{
        l = s.find(',');
        input2_of_gate = s.substr((l + 2), (k - l - 2));
    }
    return input2_of_gate;
}

string get_output_variable(string s){
    int i = s.find('(');
    int j = s.find(')');
    string a = s.substr((i + 1), (j - i - 1));
    return a;
}

// LEVELING
int gate_no(string s){
    string m=s;
    int len = m.length();
    if(len==3 && m[0]=='G'){
        int temp = m[1] + m[2] - 87;
        return temp;
    }
    else{
        int temp = m[1] - 48;
        return temp;
    }
}

// NORMAL GATES
int NOT(int a){
    return (!a);
}
int AND(int a,int b){
    return (a&&b);
}
int OR(int a,int b){
    return (a||b);
}
int XOR(int a,int b){
    if((a==0&&b==0)||(a==1&&b==1)){
        return 0;
    }
    else{
        return 1;
    }
}
int NAND(int a,int b){
    return !(a&&b);
}
int NOR(int a,int b){
    return !(a||b);
}

// TAINTED GATES
int tainted_NOT(int at){
    return at;
}
int tainted_AND(int a,int b,int at,int bt){
    p=((a&&bt)||(b&&at)||(at&&bt));
    return p;
}
int tainted_OR(int a,int b,int at,int bt){
    int a1,b1;
    a==0?a=1:a=0;
    b==0?b=1:b=0;
    
    p=((a&&bt)||(b&&at)||(bt&&at));
    return p;
}
int tainted_NAND(int a,int b,int at,int bt){
    p=((a&&bt)||(b&&at)||(at&&bt));
    return p;
}
int tainted_XOR(int a,int b,int at,int bt){
    return (at||bt);
}
int tainted_NOR(int a,int b,int at,int bt){
    int a1,b1;
    a==0?a=1:a=0;
    b==0?b=1:b=0;
    
    p=((a&&bt)||(b&&at)||(bt&&at));
    return p;
}

// EVALUATION
int eval_normal(string x,int x1,int x2=0){
    if(x=="AND"){
        return AND(x1,x2);
    }
    else if(x=="OR"){
        return OR(x1,x2);
    }
    else if(x=="XOR"){
        return XOR(x1,x2);
    }
    else if(x=="NAND"){
        return NAND(x1,x2);
    }
    else if(x=="NOR"){
        return NOR(x1,x2);
    }
    else{
        return NOT(x1);
    }
}

int eval_tainted(string x,int x1,int x2,int j2,int j3 ){
    if(x=="AND"){
        
        return tainted_AND(x1,x2,j2,j3);
    }
    else if(x=="OR"){
        return tainted_OR(x1,x2,j2,j3);
    }
    else if(x=="XOR"){
        return tainted_XOR(x1,x2,j2,j3);
    }
    else if(x=="NAND"){
        return tainted_NAND(x1,x2,j2,j3);
    }
    else if(x=="NOR"){
        return tainted_NOR(x1,x2,j2,j3);
    }    
}

// SORTING LEVEL COLUMN OF GATE INFO 
bool sortcol(vector<string>& v1, vector<string>& v2)
{
    return v1[4] < v2[4];
}

int main()
{
    int level[100]={0};
    int visited[100]={0};
    multimap <string, int> input_variables;
    multimap <string, int> output_variables;
    vector<vector<string>> gate_information;
    ifstream inFile;
    inFile.open("sample_circuit.txt");
    string line;
    while(!inFile.eof()){
        getline(inFile, line);
        if(inFile.good()){
            string s1 = line.substr(0, 5);
            string s2 = line.substr(0, 6);
            if(s1 == "INPUT"){
                string input = get_input_variable(line);
                input_variables.insert(pair <string, int> (input, 0));
            }
            else if(s2 == "OUTPUT"){
                string output = get_output_variable(line);
                output_variables.insert(pair <string, int> (output, 0));
            }
            else if (line[0] == 'G'){
                string out = gate_out(line);
                string gate = gate_name(line);
                string input1 = gate_input1(line); 
                string input2 = gate_input2(line);
                string gate_level = "0";
                vector<string> v1{out, gate, input1, input2, gate_level};
                gate_information.push_back(v1);
            }
        }
    }

    int n = gate_information.size();

    multimap <string, int> :: iterator itr;
    for (itr = input_variables.begin(); itr != input_variables.end(); ++itr)
    {
        level[(int)itr->first[1]-48] = 1;
        visited[(int)itr->first[1]-48] = 1;
        itr->second = 1; 
        cout << '\t' << itr->first
            << '\t' << itr->second << '\n';
    }
    cout << endl;

    multimap <string, int> :: iterator itr1;
    for (itr1 = output_variables.begin(); itr1 != output_variables.end(); ++itr1)
    {
        cout << '\t' << itr1->first
            << '\t' << itr1->second << '\n';
    }
    cout << endl;

    while(n>0){
        cout << endl;
        for (int i = 0; i < gate_information.size(); i++) {
            if(visited[gate_no(gate_information[i][0])]==0){
                // cout << "gate " << gate_no(gate_information[i][0]) << " ";
                if(gate_information[i][1]=="NOT"){
                    if(level[gate_no(gate_information[i][2])]>0){
                        level[gate_no(gate_information[i][0])]=level[gate_no(gate_information[i][2])]+1;
                        gate_information[i][4] = to_string(level[gate_no(gate_information[i][0])]);
                        visited[gate_no(gate_information[i][0])] = 1;
                        n-=1;
                        // cout << gate_no(gate_information[i][0]) << " ";
                    }
                }
                else{
                    if((level[gate_no(gate_information[i][2])]>0) && (level[gate_no(gate_information[i][3])]>0)){
                        level[gate_no(gate_information[i][0])] = max(level[gate_no(gate_information[i][2])], level[gate_no(gate_information[i][3])])+1;
                        gate_information[i][4] = to_string(level[gate_no(gate_information[i][0])]);
                        visited[gate_no(gate_information[i][0])] = 1;
                        n-=1;
                        // cout << gate_no(gate_information[i][0]) << " ";
                    }
                }
            }
        }
    }
    // for(int j=0;j<20;j++){
    //     cout << level[j];
    // }
    // cout << endl;

    int max_level = *max_element(level, level + 100);

    sort(gate_information.begin(), gate_information.end(), sortcol);

    cout << "GATE INFO" << endl;
    for(int i=0; i<gate_information.size(); i++){
        for(int j=0;j<5;j++){
            cout << gate_information[i][j] << " ";
        }
        cout << endl;
    }

    int result_arr[100];
    memset(result_arr, -1, sizeof(result_arr));

    // ADDING RANDOM INPUT
    for(itr = input_variables.begin(); itr != input_variables.end(); ++itr){
        // cout << (int)itr->first[1]-48 << " ";
        int value = ((int)itr->first[1]-48)%2;
        result_arr[(int)itr->first[1]-48] = value;
    }

    cout << "INPUT" << endl;
    for(int i=0; i < 20; i++){
        cout << "G" << i << "(" << result_arr[i] << ") ";
    }
    cout << endl;

    for(int i=0;i<gate_information.size();i++){
        if(gate_information[i][1]=="NOT"){
            result_arr[gate_no(gate_information[i][0])] = eval_normal(gate_information[i][1], result_arr[gate_no(gate_information[i][2])]);
        }
        else{
            result_arr[gate_no(gate_information[i][0])] = eval_normal(gate_information[i][1], result_arr[gate_no(gate_information[i][2])], result_arr[gate_no(gate_information[i][3])]);
        }
        // tainted_arr[gate_no(gate_information[i][0])] = result_arr[gate_no(gate_information[i][0])];
        if( output_variables.find(gate_information[i][0]) != output_variables.end()){
            cout << "TAINTED INPUT TRUTH TABLE FOR GATE " << gate_information[i][0] << " :" << endl;
            if(gate_information[i][1]=="NOT"){
                cout << "a at output" << endl;
                for(int j=0;j<2;j++){
                    cout << result_arr[gate_no(gate_information[i][2])] << " " << j << " " << tainted_NOT(j) << endl;
                }
                cout << endl;
            }
            else{
                cout << "a b at bt output" << endl;
                for(int j=0;j<2;j++){
                    for(int k=0;k<2;k++){
                        cout << result_arr[gate_no(gate_information[i][2])] << " " << result_arr[gate_no(gate_information[i][3])] << " " << j << " " << k << " " << eval_tainted(gate_information[i][1], result_arr[gate_no(gate_information[i][2])], result_arr[gate_no(gate_information[i][3])], j, k) << endl;
                    }
                }
                cout << endl;
            }
        }
    }

    cout << "FINAL" << endl;
    for(int i=0; i < 20; i++){
        cout << "G" << i << "(" << result_arr[i] << ") ";
    }
    cout << endl;

    return 0;
}