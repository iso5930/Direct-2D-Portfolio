#ifndef Define_h__
#define Define_h__

#define	DECLARE_SINGLETON(ClassName)		\
private:									\
	static ClassName*	m_pInstance;		\
public:										\
	static ClassName*	GetInstance(void)	\
	{										\
		if(m_pInstance == NULL)				\
			m_pInstance = new ClassName;	\
		return	m_pInstance;				\
	}										\
	void	DestroyInstance(void)			\
	{										\
		if(m_pInstance)						\
		{									\
			delete m_pInstance;				\
			m_pInstance = NULL;				\
		}									\
	}										\
private:									\
	ClassName(ClassName& rName){;}			\


#define	IMPLEMENT_SINGLETON(ClassName)		\
ClassName*	ClassName::m_pInstance = NULL;	\

#define ERR_MSG(text) MessageBox(NULL, L"Error", text, MB_OK)

#define GETTIME CTimeMgr::GetInstance()->GetTime()

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }

#endif // Define_h__
