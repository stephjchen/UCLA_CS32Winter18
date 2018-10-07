
class Investment //base class, ABC
{
public:
    
    Investment (string name, int price)
    {
        m_name = name;
        m_price = price;
    }
    
    virtual ~Investment(){}
    
    string name() const
    {
        return m_name;
    }
    
    int purchasePrice() const
    {
        return m_price;
    }
    
    virtual bool fungible () const
    {
        return false;
    }
    
    virtual string description() const = 0;
    
private:
    
    string m_name;
    int m_price;
    
};

class Painting : public Investment
{
public:
    
    Painting (string name, int price): Investment(name, price){}
    
    virtual ~Painting ()
    {
        cout << "Destroying " << name() << ", a " << description() << endl;
    }
    
    virtual string description() const
    {
        return "painting";
    }
    
private:
};

class Stock : public Investment
{
public:
    
    Stock (string name, int value, string tickerSymbol): Investment(name, value), m_tickerSymbol(tickerSymbol){}
    
    virtual ~Stock ()
    {
        cout << "Destroying " << name() << ", a stock holding" << endl;
    }
    
    virtual bool fungible () const
    {
        return true;
    }
    
    virtual string description() const
    {
        string s = "stock trading as " + m_tickerSymbol;
        return s;
    }
    

private:
    string m_tickerSymbol;
};

class House : public Investment
{
    
public:
    
    House (string name, int value): Investment(name, value){}
    
    virtual ~House ()
    {
        cout << "Destroying the " << description() << " " << name() << endl;
    }
    
    virtual string description() const
    {
        return "house";
    }

private:
};

