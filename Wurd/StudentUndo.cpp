#include "StudentUndo.h"

Undo* createUndo()
{
	return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
	// TODO
    
    undoStack toSubmit;
    
    toSubmit.m_action = action;
    toSubmit.m_row = row;
    toSubmit.m_col = col;
    
    if(ch == '\t')
        toSubmit.m_string += "    ";
    else
        toSubmit.m_string += ch;
    
    m_stack.push(toSubmit);
    
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    // TODO
    
    if(m_stack.empty())
        return ERROR;
        
    undoStack undo = m_stack.top();
    
    m_stack.pop();
    
    Action act = undo.m_action;
    
    if(act == INSERT)
    {
        row = undo.m_row;
        col = undo.m_col;
        count = undo.m_count;
        text = "";
        
        return DELETE;
    }
    else if(act == DELETE)
    {
        row = undo.m_row;
        col = undo.m_col;
        count = 1;
        text = undo.m_string;
        
        return INSERT;
    }
    else if(act == JOIN)
    {
        row = undo.m_row;
        col = undo.m_col;
        count = 1;
        text = "";
        
        return SPLIT;
    }
    else if (act == SPLIT)
    {
        row = undo.m_row;
        col = undo.m_col;
        count = 1;
        text = "";
        
        return JOIN;
    }
    
    return ERROR;
}

void StudentUndo::clear() {
    
    while(!m_stack.empty())
        m_stack.pop();
    
	// TODO
}
