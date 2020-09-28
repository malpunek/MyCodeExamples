//Stanislaw Morawski
//
//Implementation of Ukkonen's algorithm for online linear construction of suffix trees
//
//Assuming input is a word over alphabet (a.....z)

using namespace std;

const int ALPHABET_SIZE = 26;
const char FIRST_LETTER = 'a';

struct node{
	int left;
	int right;
	node * fail;
	node * child[ALPHABET_SIZE];
    node(int left, int right) : left(left), right(right){
		for (int i = 0; i < ALPHABET_SIZE; i++)
			child[i]=NULL;
		}
    };

//for character k returns its representative in int
inline int charToInt(char const& c){
	return (int) (c - FIRST_LETTER);
	}

node * constructSuffixTree(string const& s){
    node * root = new node(-1, 0);
	root-> fail = new node(0, 0);
    for (int i = 0; i < ALPHABET_SIZE; i++)
		(root -> fail) -> child[i] = root;
	node * active = root;
    node * edge;
    node * lastleaf = root -> fail;
	int begin = 0, head = 0, tail = 0;
	const int end = (int) s.size();
	for (int done = begin; done < end; ++done){
	    node * lastnode = root -> fail;
		while (head <= done){
			edge = active -> child[charToInt(s[tail])];
			while (edge != NULL and (edge -> right) - (edge -> left) <= (done - tail)){
				active = edge;
				tail += (edge -> right) - (edge -> left);
				edge = active -> child[charToInt(s[tail])];
				}
			if (edge != NULL and s[edge -> left + done - tail] == s[done]){
                lastnode -> fail = active;
                break;
                }
            node * divide = active;
			if (edge!=NULL){
				divide = new node(edge -> left, edge -> left + done - tail);
                active -> child[charToInt(s[divide -> left])] = divide;
                divide -> child[charToInt(s[divide -> right])] = edge;
                edge -> left = divide -> right;
                }
            lastleaf = lastleaf -> fail = divide -> child[charToInt(s[done])] = new node(done, end);
            lastnode = lastnode -> fail = divide;
            head++;
            active = active -> fail;
            }
        }
	return root;
	}
