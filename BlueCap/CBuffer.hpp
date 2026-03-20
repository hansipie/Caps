class CBuffer
{
public:
  CBuffer(char *_buff, int _size)
  {
    //cout << "Constructeur CBuffer\n";
    size = _size;
    for(int i=0; i<size; i++)
      buffer.push_back(_buff[i]);
    charbuff = NULL;
  }
  char *getbuff()
  {
    int i=0;
    charbuff = new char[size];
    //cout << "CBuffer::getbuff() - malloc" << endl;
    for (vector<char>::iterator it = buffer.begin(); it != buffer.end(); it++)
      charbuff[i++] = *it;
    return charbuff;
  }
  void delbuff()
  {
    //cout << "CBuffer::delbuff() - free" << endl;
    delete [] charbuff;
  }
  char *charbuff;
  vector<char> buffer;
  int size;
};

class CBufferTo : public CBuffer
{
public:
  CBufferTo(int _to, char *_buff, int _size):CBuffer(_buff, _size)
  {
    to = _to;
  }
  int to;
};

