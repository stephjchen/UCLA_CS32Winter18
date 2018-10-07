void listAll(const Class* c, string path)  // two-parameter overload
{
    if (path == "")
        path += c->name();
    
    else
        path += "=>" + c->name();
    
    cout << path << endl;
    
    vector<Class*> :: const_iterator it;
    for (it = c->subclasses().begin(); it != c->subclasses().end(); it++)
        listAll((*it), path);
}

