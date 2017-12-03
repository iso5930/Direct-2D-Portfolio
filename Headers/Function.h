#ifndef Function_h__
#define Function_h__

inline	wstring	ConvertRelativePath(wstring szFullPath)
{
	TCHAR	szRelativePath[MAX_PATH] = L"";
	TCHAR	szDirectoryPath[MAX_PATH] = L"";

	//vcproj 파일이 있는 위치까지의 경로를 얻는 함수!!
	GetCurrentDirectory(sizeof(szDirectoryPath), szDirectoryPath);

	//한 경로에서 다른 경로로 가는 상대 경로를 추출하는 함수...
	PathRelativePathTo((LPWSTR)szRelativePath, szDirectoryPath,
		FILE_ATTRIBUTE_DIRECTORY, szFullPath.c_str(), FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}


class CStringCmp
{
private:
	const TCHAR*		m_pName;

public:
	explicit CStringCmp(const TCHAR* pKey)
		:m_pName(pKey){}

public:
	template<typename T>
	bool operator () (T data)
	{
		return (!lstrcmp(data.first, m_pName));
	}
};


#endif // Function_h__
