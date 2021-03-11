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
    {
        toSubmit.m_string += "    ";
        toSubmit.m_count = 4;
    }
    else
        toSubmit.m_string += ch;
    
    if(!m_stack.empty())
    {
        int positionCol = m_stack.top().m_col - toSubmit.m_col;
        int positionRow = m_stack.top().m_row - toSubmit.m_row;
        
        if(action == INSERT && m_stack.top().m_action == INSERT && positionRow == 0)
        {
            if(positionCol == -1 || (toSubmit.m_string == "    " && m_stack.top().m_string == "    " && positionCol == -4) || (toSubmit.m_string == "    " && positionCol == -4) )
            {
                toSubmit.m_count += m_stack.top().m_count;
                
                m_stack.pop();
                m_stack.push(toSubmit);
                return;
            }
        }
        
        else if(action == DELETE && m_stack.top().m_action == DELETE && positionRow == 0)
        {
            if(positionCol == 0)
            {
                toSubmit.m_string = m_stack.top().m_string + toSubmit.m_string;
                m_stack.pop();
                m_stack.push(toSubmit);
                return;
            }
            else if (positionCol == 1)
            {
                toSubmit.m_string += m_stack.top().m_string;
                m_stack.pop();
                m_stack.push(toSubmit);
                return;
            }
        }
    }

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
