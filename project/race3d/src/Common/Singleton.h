#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <class T>
class Singleton
{
public:
	static T* GetInstance();
	void FreeInstance();

protected:
	Singleton() {}
	virtual ~Singleton()
	{
		m_self = 0;
	}

private:
	static T*	m_self;
	static int	m_refcount;
};

template <class T>
T* Singleton<T>::m_self = 0;

template <class T>
int Singleton<T>::m_refcount = 0;

template <class T>
T* Singleton<T>::GetInstance()
{
	if( !m_self )
	{
		m_self = new T;
	}

	m_refcount++;

	return m_self;
}

template <class T>
void Singleton<T>::FreeInstance()
{
	if( --m_refcount == 0 )
	{
		delete this;
	}
}

#endif