#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include "Undo.h"
#include <stack>
#include <string>

using namespace std;

class StudentUndo : public Undo {
public:

	void submit(Action action, int row, int col, char ch = 0);
	Action get(int& row, int& col, int& count, std::string& text);
	void clear();

private:
    
    struct undoStack
    {
        Action m_action;
        int m_row = 0;
        int m_col = 0;
        string m_string = "";
        int m_count = 1;
    };
    
    stack<undoStack> m_stack;
};

#endif // STUDENTUNDO_H_
