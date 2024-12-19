
#include <bits/stdc++.h>

using namespace std;

//hatzabat values el gadwal

void getBaseValues(vector <map <char,bool>> &iVal , vector <char> inputs ,int nRows){
    
    int nInputs = inputs.size();
    
    for (int i = 0; i < nRows; i++) {
        map<char, bool> row; 
        for (int j = 0; j < inputs.size(); j++) {
            
            bool value = (i >> (nInputs - j - 1)) & 1;
            row[inputs[j]] = value; 
        }
        iVal.push_back(row);
    }
    
}



bool solveLogicExpression(vector<char> expression , map<char,bool> values , int s , int e){
    
    stack<bool> operands; 
    stack<char> operators;
    int b1c = 0, b2c= 0, b1i = -1 , b2i= -1;
    bool br = true;

    if (expression.size() == 1 ) return values[expression[0]];
    else if (expression.size() == 2 && expression[0] == '~') return !values[expression[1]];
    for (int i = s ; i<= e ; i++){
        
        char ch = expression[i];

        if (br && b1c > b2c){

            
            if (ch== '(') b1c++;
            else if (ch == ')'){
                b2c++;
                b2i= i ;
                if (b2c == b1c){
                    int ansbr = solveLogicExpression(expression, values , b1i+1 , b2i-1);
                    operands.push(ansbr);
                    br = false;
                }
            }
            else continue;
        }

        //seperate the operands and operators from expression
        if (ch == '('){
            b1i = i;
            b1c++;
            br = true;
        
        }else if (values.count(ch) != 0){
            operands.push(values[ch]);
        }else if ( ch == '~'){
            
            ch = expression[i+1];
            if (i <= e && values.count(ch) != 0){
                bool neg = !values[ch];
                operands.push(!values[ch]);
                i++;
                
            }
        }else if (ch == '^' || ch == '|') {
            operators.push(ch);
        }else if (ch == '-' && expression[i+1] == '>'){            
            operators.push('>');
            i++;
            
        }
        
        //solve operation if there is atleast 2 operands and 1 operator
        
        while ( !operators.empty() && 2 <= operands.size()){
            bool op2 = operands.top();// first operand
            operands.pop();
            bool op1 = operands.top();// second operand
            operands.pop();
            char oper = operators.top();// operator
            operators.pop();
            
            if (oper == '|') operands.push(op1 || op2);
            else if (oper == '^') operands.push(op1 && op2);
            else if (oper == '>') operands.push((!op1) || op2 );
        }
    }
    
    
    if (!operands.empty())
    return operands.top();
    else  return false;
    
}

char TF(bool ans){
    if (ans){
        return 'T';
    }else{
        return 'F';
    }
}

void printTT(vector<map <char,bool>> iVal ,vector<char> inputs , vector<vector<char>> premises, vector<char> conclusion , int nPremises, int nInputs, int nRows){
    
    int nColoumns = nPremises + nInputs + 1 ;
    bool val = false, valid=false;
    bool sat = false, satisfiable = false ;
    cout<<"\nTruthTable: (CR*:critical rows)\n";
    cout<<" ";
    for (int i = 0 ; i<nInputs ; i++){
        
        cout<< inputs[i] <<"\t";
    }
    
    for (int i = 0 ; i<nPremises ; i++){
        cout<< "p" << i+1 <<"\t";
    }
    
    cout<< 'c' << endl;
    
    int r = 0 ;
    bool vs = true;
    
    for (int i = 0 ; i<nRows; i++){
        
         
        
        
            cout<<" ";
            sat = true;
            val = false;
            for (int k = 0 ; k<nInputs ; k++){
                bool ans = iVal[i][inputs[k]];
                cout<< TF(ans) <<"\t";
                
                // cout<< premises[j][k] << iVal[i][inputs[k]] <<"\t";
            }
            
            for (int k = 0 ; k<nPremises ; k++){
                
               bool ans = solveLogicExpression(premises[k],iVal[i], 0 , premises[k].size()-1);
                cout<< TF(ans) <<"\t";
                // cout<< premises[j][k] << iVal[i][inputs[k]] <<"\t";
                sat = sat && ans;
            }
            
            
            
            bool ans = solveLogicExpression(conclusion,iVal[i], 0 , conclusion.size()-1);
                cout<< TF(ans) <<" ";
            val = sat && ans;
            
            if (sat && !ans) vs=false;
            if (sat){
                satisfiable = true;  
                cout << " CR* ";
            }
            
            if (val && vs) valid = true;
            
            
            cout<<endl;
        
        
        
    }
    cout<<endl;
    cout<<"Argument is ";
    if (valid && vs){
            cout<< "valid";
    }else {
        cout<<"falsifiable";
    }
        // cout<<endl;
        cout<<" and ";
    if (satisfiable){
            cout<< "satisfiable."<<endl;
    }else {
        cout<< "not satisfiable."<<endl;
    }
    
    
}




void solution(vector<char> inputs , vector<vector<char>> premises, vector<char> conclusion , int nPremises, int nInputs){
    
    int nRows = pow(2,nInputs);
    int nColoumns = nPremises + nInputs + 1 ;
    vector<map <char,bool>> iVal;
    
    getBaseValues(iVal,inputs,nRows);
    
    printTT(iVal, inputs , premises , conclusion, nPremises , nInputs , nRows);
    
    
}

int main()
{
    int nPremises , nInputs;
    
    cout<< "enter number of inputs: ";
    cin>>nInputs;
    cout<< "enter number of premises: ";
    cin>>nPremises;
    
    vector<char> inputs;
    vector<vector<char>> premises(nPremises);
    vector <char> conclusion;
    
    cout<< "enter inputs(ex:a b):";
    //get inputs
    for (int i =0 ; i<nInputs ; i++){
        char y;
        cin>>y;
        inputs.push_back(y);
        
    }
    
    
    //get premises
    cout<< "enter premises of the argument('~':not,'^':and,'->':if-then,'|':or,',':separate premises, '.':end):\n";
    for (int i = 0 ; i<nPremises ;i++){
        char y = ' '; 
        while (y != ',' && y != '.'){
            cin>>y;
            if (y != ' ' && y != ',' && y != '.'){
                premises[i].push_back(y);
            }
            
        }
    }
    
    cout<< "enter conclusion of the argument('.' to end):\n ";
    
    char y = ' '; 
    while (y != '.'){
        cin>>y;
        if (y != ' ' && y != '.'){
            conclusion.push_back(y);
        }
        
    }
    // for (char x : conclusion){
    //     cout<<x;
    // }
     solution(inputs, premises, conclusion,nPremises,nInputs);

    
}

/*
3
2
K W D
(K | W) -> D , W|D.
~K->~D.

2
2
p q 
~p->~q, p.
q.

2
3
p q 
(p | q) -> q, ~q -> p, p ^ q.
q.

2
1
x y 
x -> ~y.
y.

valid
3
2
P Q R 
P -> Q, Q -> R.
P -> R.

not satisfiable
3
2
P Q R 
P ^ Q, ~Q.
P.


*/
