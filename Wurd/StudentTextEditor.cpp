#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

TextEditor* createTextEditor(Undo* un)
{
	return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
 : TextEditor(undo) {
	// TODO
     m_list.push_back("");
     m_it = m_list.begin();
     m_currentRow = 0;
     m_currentColumn = 0;
}

StudentTextEditor::~StudentTextEditor()
{
    m_list.clear();
	// TODO
}

bool StudentTextEditor::load(std::string file) {
    
    reset();
    
    ifstream infile("/Users/wherlim18/Desktop/Wurd/threemen.txt");    // infile is a name of our choosing
        if ( ! infile )                // Did opening the file fail?
        {
            cerr << "Error: Cannot open data.txt!" << endl;
            return false;
        }
    
    string s;
    // getline returns infile; the while tests its success/failure state
    while (getline(infile, s))
    {
        if(!s.empty() && s[s.size()-1] == '\r')
            s.erase(s.size()-1);
        m_list.push_back(s);
    }

    m_currentRow = 0;
    m_currentColumn = 0;
    m_it = m_list.begin();
    
    return true;  // TODO
}

bool StudentTextEditor::save(std::string file) {
    ofstream outfile("/Users/wherlim18/Desktop/Wurd/threemen.txt");   // outfile is a name of our choosing.
        if ( ! outfile )           // Did the creation fail?
        {
            cerr << "Error: Cannot create results.txt!" << endl;
            return false;
        }
    
    list<string>::iterator li = m_list.begin();
    
    while(li != m_list.end())
    {
        outfile << *li << '\n';
        li++;
    }
	return true;  // TODO
}

void StudentTextEditor::reset() {
    m_list.clear();
    m_currentRow = 0;
    m_currentColumn = 0;
}

void StudentTextEditor::move(Dir dir) {

    m_it = m_list.begin();
    
    int pos = 0;
    
    while(m_it != m_list.end() && pos < m_currentRow)
    {
        m_it++;
        pos++;
    }
    
    if(dir == UP)
    {
        if(m_currentRow == 0)
            return;
        else
        {
            m_currentRow--;
            m_it--;
            if(m_currentColumn >= (*m_it).size())
                m_currentColumn = (*m_it).size();
        }
    }
    else if (dir == DOWN)
    {
        if(m_currentRow >= m_list.size()-1)
            return;
        else
        {
            m_currentRow++;
            m_it++;
            if(m_currentColumn >= (*m_it).size())
                m_currentColumn = (*m_it).size();
        }
    }
    else if (dir == LEFT)
    {
        if(m_currentColumn == 0)
        {
            if(m_currentRow == 0)
                return;
            else
            {
                m_currentRow--;
                m_it--;
                m_currentColumn = (*m_it).size();
            }
        }
        else
        {
            m_currentColumn--;
        }
    }
    else if (dir == RIGHT)
    {
        if(m_currentColumn == (*m_it).size())
        {
            if(m_currentRow >= m_list.size()-1)
                return;
            else
            {
                m_currentRow++;
                m_it++;
                m_currentColumn = 0;
            }
        }
        else
            m_currentColumn++;
    }
    else if (dir == HOME)
    {
        if((*m_it).size() == 0)
            return;
        else
            m_currentColumn = 0;
    }
    else if (dir == END)
    {
        if((*m_it).size() == 0)
            return;
        else
            m_currentColumn = (*m_it).size();
    }
}

void StudentTextEditor::del() {
    
    if(m_currentColumn == (*m_it).size() || (*m_it).size() == 0)
    {
        if(m_currentRow >= m_list.size()-1)
            return;
        
        string s1 = (*m_it);
        m_it = m_list.erase(m_it);
        string s2 = (*m_it);
        (*m_it) = s1+s2;
        
        getUndo()->submit(Undo::JOIN, m_currentRow, m_currentColumn);
    }
    else
    {
        char ch = (*m_it)[m_currentColumn];
        (*m_it).erase(m_currentColumn, 1);
        getUndo()->submit(Undo::DELETE, m_currentRow, m_currentColumn, ch);
    }
}

void StudentTextEditor::backspace() {
    if(m_currentColumn > 0)
    {
        char ch = (*m_it)[m_currentColumn-1];
        (*m_it).erase(m_currentColumn-1, 1);
        m_currentColumn--;
        getUndo()->submit(Undo::DELETE, m_currentRow, m_currentColumn, ch);
    }
    else if(m_currentColumn == 0 || (*m_it).size() == 0)
    {
        if(m_currentRow == 0)
            return;
        
        string s1 = (*m_it);
        m_it = m_list.erase(m_it);
        m_it--;
        m_currentColumn = (*m_it).size();
        m_currentRow--;
        (*m_it) = (*m_it) + s1;
        
        getUndo()->submit(Undo::JOIN, m_currentRow, m_currentColumn);
    }
}

void StudentTextEditor::insert(char ch) {
    
    if(ch == '\t')
    {
        (*m_it).insert(m_currentColumn, "    ");
        m_currentColumn += 4;
        getUndo()->submit(Undo::INSERT, m_currentRow, m_currentColumn, '\t');
    }
    else
    {
        (*m_it).insert(m_currentColumn, 1, ch);
        m_currentColumn++;
        getUndo()->submit(Undo::INSERT, m_currentRow, m_currentColumn, ch);
    }
}

void StudentTextEditor::enter() {
    
    int tempRow = m_currentRow;
    int tempColumn = m_currentColumn;
    
    string s1 = (*m_it).substr(0, m_currentColumn);
    string s2 = (*m_it).substr(m_currentColumn);
    
    (*m_it) = s1;
    
    m_it++;
    m_list.insert(m_it, s2);
    m_it--;
    
    m_currentRow++;
    m_currentColumn = 0;
    
    getUndo()->submit(Undo::SPLIT, tempRow, tempColumn);
	// TODO
}

void StudentTextEditor::getPos(int& row, int& col) const {
    // TODO
    row = m_currentRow;
    col = m_currentColumn;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
	
    if(startRow < 0 || numRows < 0 || startRow > m_list.size())
        return -1;
    
    list<string>::const_iterator li = m_list.begin();
    
    for(int i = 0; i < startRow && li != m_list.end(); i++)
        li++;
    
    lines.clear();
    
    int counter = 0;
    
    while(li != m_list.end() && counter < numRows)
    {
        lines.push_back(*li);
        li++;
        counter++;
    }
    
    return lines.size();
}

void StudentTextEditor::undo() {
	// TODO

    int testRow;
    int testCol;
    int testCount;
    string testString;
    
    Undo::Action a = getUndo()->get(testRow, testCol, testCount, testString);
    
    if(a == Undo::INSERT)
    {
        int distance = testRow - m_currentRow;
        
        if(distance < 0)
        {
            for(int i = 0; i < distance*-1; i++)
                m_it--;
        }
        else if (distance > 0)
        {
            for(int i = 0; i < distance; i++)
                m_it++;
        }
        
        (*m_it).insert(testCol, testString);
        
        m_currentRow = testRow;
        m_currentColumn = testCol+1;
    }
    else if (a == Undo::DELETE)
    {
        int distance = testRow - m_currentRow;
        
        if(distance < 0)
        {
            for(int i = 0; i < distance*-1; i++)
                m_it--;
        }
        else if (distance > 0)
        {
            for(int i = 0; i < distance; i++)
                m_it++;
        }
        
        for(int i = 0; i < testCount; i++)
        {
            (*m_it).erase(testCol-1, 1);
            m_currentColumn--;
        }
        
        m_currentRow = testRow;
        m_currentColumn = testCol-1;
    }
    else if(a == Undo::SPLIT)
    {
        int distance = testRow - m_currentRow;
        
        if(distance < 0)
        {
            for(int i = 0; i < distance*-1; i++)
                m_it--;
        }
        else if (distance > 0)
        {
            for(int i = 0; i < distance; i++)
                m_it++;
        }
    
        m_currentRow = testRow;
        m_currentColumn = testCol;
        
        string s = (*m_it).substr(m_currentColumn);
        (*m_it).erase(m_currentColumn, (*m_it).size() - m_currentColumn);
        
        m_it++;
        m_list.insert(m_it, s);
        m_it--;
        m_it--;
        
        
    }
    else if(a == Undo::JOIN)
    {
        int distance = testRow - m_currentRow;
        
        if(distance < 0)
        {
            for(int i = 0; i < distance*-1; i++)
                m_it--;
        }
        else if (distance > 0)
        {
            for(int i = 0; i < distance; i++)
                m_it++;
        }
        
        
        m_it++;
        
        m_currentRow = testRow;
        m_currentColumn = testCol;
        
        string s1 = (*m_it);
        m_it = m_list.erase(m_it);
        m_it--;
        string s2 = (*m_it);
        (*m_it) = s2+s1;
    }
}
