#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#pragma once

//#define __KEY_BLOB_IN_REGISTRY__
#define __KEY_BLOB_IN_INIITIALIZATIONFILE__

#include "winlib\winobj_base.hpp"
#include <wincrypt.h>
#include <exception>
#include <stdio.h>
#include <string>

#ifdef __KEY_BLOB_IN_REGISTRY__
	#include "RegKeyWriter.hpp"
	#include "RegKeyReader.hpp"
	#include "Hexer.h"
#endif	// __KEY_BLOB_IN_REGISTRY__

#ifdef __KEY_BLOB_IN_INIITIALIZATIONFILE__
    #include "Hexer.h"
#endif //__KEY_BLOB_IN_INIITIALIZATIONFILE__


namespace winobj {

	class crypt_exception: public exception {
		public:
			crypt_exception() {
				m_error = GetLastError();
                char* p_msg;
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								NULL, m_error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&p_msg, 0, 0);
                m_what = p_msg;
                LocalFree(p_msg);
			}
			crypt_exception(DWORD error_code) {
				m_error = error_code;
                char* p_msg;
				FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&p_msg, 0, 0);
                m_what = p_msg;
                LocalFree(p_msg);
			}

			crypt_exception& operator=(const crypt_exception& e) {
				m_error = e.m_error;
                m_what = e.m_what;
				return (*this);
			}

			virtual ~crypt_exception() {}

			const char* what() const {
                return m_what.c_str();
            }

			DWORD err() {
                return m_error;
            }
		private:
			DWORD m_error;
			string m_what;
	};

	class TripleDesCtx {
		public:
			TripleDesCtx(const char* container_name):m_prov(0) {
				if (CryptAcquireContext(&m_prov, container_name,
												MS_ENHANCED_PROV, PROV_RSA_FULL, 0) == FALSE) {
					if (CryptAcquireContext(&m_prov, container_name,
													MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET) == FALSE) {
	 					throw crypt_exception();
					} else {
						HCRYPTKEY skey(0), ekey(0);
						if (CryptGenKey(m_prov, AT_SIGNATURE, 0, &skey) == TRUE) {
							if (CryptGenKey(m_prov, AT_KEYEXCHANGE, 0, &ekey) == TRUE) {
								CryptDestroyKey(ekey);
							} else {
			 					crypt_exception e;
								DeleteKeySet(container_name);
								throw e;
							}
							CryptDestroyKey(skey);
						} else {
							crypt_exception e;
							DeleteKeySet(container_name);
		 					throw e;
						}
					}
				}

				// create Exchange Key & Signature key
			}
			~TripleDesCtx() { if (m_prov) CryptReleaseContext(m_prov, 0); }
			operator HCRYPTPROV() { return m_prov; }

		private:
			void DeleteKeySet(const char* name) {
				if (CryptAcquireContext(&m_prov, name,
										MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_DELETEKEYSET) == FALSE)
					throw crypt_exception();
			}
		private:
			HCRYPTPROV m_prov;
	};

	class TripleDesKey {
		public:
			TripleDesKey();
			TripleDesKey(TripleDesCtx* ctx):p_ctx(ctx), m_key(0) {}
			virtual ~TripleDesKey() { if (m_key) CryptDestroyKey(m_key); }
			operator HCRYPTKEY() { return m_key; }
			void create() {
				if (CryptGenKey(*p_ctx, CALG_3DES_112, (128 << 16) | CRYPT_EXPORTABLE, &m_key) == FALSE) {
					throw crypt_exception();
				}
			}

			bool loadKey(const char* filename) {
				BYTE* keyblob = 0;
				FILE* inf = fopen(filename, "rb");
				if (!inf) return false;
				fseek(inf, 0L, SEEK_END);
				DWORD len = ftell(inf);
				keyblob = new BYTE[len];
				fseek(inf, 0, SEEK_SET);
				fread(keyblob, len, 1, inf);
				fclose(inf);

				HCRYPTKEY ukey;
				if (CryptGetUserKey(*p_ctx, AT_KEYEXCHANGE, &ukey) == FALSE) {
					throw crypt_exception();
				}
				HCRYPTKEY tkey = 0;
				if (CryptImportKey(*p_ctx, keyblob, len, ukey, CRYPT_EXPORTABLE, &tkey) == FALSE) {
					delete [] keyblob;
					crypt_exception e;
					CryptDestroyKey(ukey);
					throw e;
				}
				delete [] keyblob;
				CryptDestroyKey(ukey);
				CryptDestroyKey(m_key);
				m_key = tkey;
				return true;
			}

			bool storeKey(const char* filename) {
				if (!m_key) return false;
				HCRYPTKEY ukey;
				if (CryptGetUserKey(*p_ctx, AT_KEYEXCHANGE, &ukey) == FALSE) {
					throw crypt_exception();
				}
				DWORD size;
				if (CryptExportKey(m_key, ukey, SIMPLEBLOB, 0, 0, &size) == FALSE) {
					crypt_exception e;
					CryptDestroyKey(ukey);
					throw e;
				}
				BYTE* keyblob = new BYTE[size];
				if (CryptExportKey(m_key, ukey, SIMPLEBLOB, 0, keyblob, &size) == FALSE) {
					crypt_exception e;
					delete [] keyblob;
					CryptDestroyKey(ukey);
					throw e;
				}
				CryptDestroyKey(ukey);
				FILE* of = fopen(filename,"wb");
				if (of) {
					fwrite(keyblob, size, 1, of);
					fclose(of);
					delete [] keyblob;
					return true;
				}
				delete [] keyblob;
				return false;
			}

			DWORD encrypt(BYTE* buffer, DWORD data_len, DWORD buffer_len, bool final) {
				DWORD len = data_len;
				if (CryptEncrypt(m_key, 0, final ? TRUE : FALSE, 0, buffer, &len, buffer_len) == TRUE)
					return len;
				throw crypt_exception();
			}

			DWORD decrypt(BYTE* buffer, DWORD data_len, bool final) {
				DWORD len = data_len;
				if (CryptDecrypt(m_key, 0, final ? TRUE : FALSE, 0, buffer, &len) == TRUE)
					return len;
				throw crypt_exception();
			}

#ifdef __KEY_BLOB_IN_REGISTRY__
	public:
			bool storeKey(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName = NULL);
			bool loadKey(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName = NULL);
#endif	// __KEY_BLOB_IN_REGISTRY__

#ifdef __KEY_BLOB_IN_INIITIALIZATIONFILE__
	public:
			bool storeKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszFileName);
			bool loadKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszFileName);
#endif //__KEY_BLOB_IN_INIITIALIZATIONFILE__

	protected:
//		private:
			TripleDesCtx*	p_ctx;
			HCRYPTKEY m_key;
	};

#ifdef __KEY_BLOB_IN_INIITIALIZATIONFILE__
	inline bool TripleDesKey::storeKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszFileName)
	{
		if (!m_key) return false;
		HCRYPTKEY ukey;
		if (CryptGetUserKey(*p_ctx, AT_KEYEXCHANGE, &ukey) == FALSE) {
			throw crypt_exception();
		}
		DWORD size;
		if (CryptExportKey(m_key, ukey, SIMPLEBLOB, 0, 0, &size) == FALSE) {
			crypt_exception e;
			CryptDestroyKey(ukey);
			throw e;
		} 
		BYTE* keyblob = new BYTE[size];
		if (CryptExportKey(m_key, ukey, SIMPLEBLOB, 0, keyblob, &size) == FALSE) {
			crypt_exception e;
			delete [] keyblob;
			CryptDestroyKey(ukey);
			throw e;
		} 
		CryptDestroyKey(ukey);

		std::string s;
		Hexer hex;
		hex.set((char*)keyblob, size);
		s = hex.toString();

		delete [] keyblob;
		if (!WritePrivateProfileString(lpszSectionName, lpszKeyName, s.c_str(), lpszFileName))
			throw crypt_exception();
		return true;
	}
	
	inline bool TripleDesKey::loadKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszFileName)
	{
		BYTE* keyblob = 0;

		LONG rc;
		char* szvalue = new char[512];
		DWORD dwsize = 512;
		rc = GetPrivateProfileString(lpszSectionName, lpszKeyName, "", szvalue, dwsize, lpszFileName);
		if (rc == 0) {
			if (dwsize <= 512) {
				delete [] szvalue;
				throw crypt_exception(rc);
			}
			else {
				delete [] szvalue;
				szvalue = new char[dwsize];
				rc = GetPrivateProfileString(lpszSectionName, lpszKeyName, "", szvalue, dwsize, lpszFileName);
				if (rc != ERROR_SUCCESS) {
					delete [] szvalue;
					throw crypt_exception(rc);
				}
			}
		}
		DWORD len;
		
		std::string s(szvalue);
		Hexer hex(s);
		len = hex.getlength();
		keyblob = new BYTE[len];
		hex.copyContent((char*)keyblob, len);

		delete [] szvalue;

		HCRYPTKEY ukey;
		if (CryptGetUserKey(*p_ctx, AT_KEYEXCHANGE, &ukey) == FALSE) {
			delete [] keyblob;
			throw crypt_exception();
		}
		HCRYPTKEY tkey = 0;
		if (CryptImportKey(*p_ctx, keyblob, len, ukey, CRYPT_EXPORTABLE, &tkey) == FALSE) {
			delete [] keyblob;
			crypt_exception e;
			CryptDestroyKey(ukey);
			throw e;
		}
		delete [] keyblob;
		CryptDestroyKey(ukey);
		CryptDestroyKey(m_key);
		m_key = tkey;
		return true;
	}
#endif //__KEY_BLOB_IN_INIITIALIZATIONFILE__

#ifdef __KEY_BLOB_IN_REGISTRY__
	inline bool TripleDesKey::storeKey(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName)
	{
		if (!m_key) return false;
		HCRYPTKEY ukey;
		if (CryptGetUserKey(*p_ctx, AT_KEYEXCHANGE, &ukey) == FALSE) {
			throw crypt_exception();
		}
		DWORD size;
		if (CryptExportKey(m_key, ukey, SIMPLEBLOB, 0, 0, &size) == FALSE) {
			crypt_exception e;
			CryptDestroyKey(ukey);
			throw e;
		} 
		BYTE* keyblob = new BYTE[size];
		if (CryptExportKey(m_key, ukey, SIMPLEBLOB, 0, keyblob, &size) == FALSE) {
			crypt_exception e;
			delete [] keyblob;
			CryptDestroyKey(ukey);
			throw e;
		} 
		CryptDestroyKey(ukey);

		std::string s;
		Hexer hex;
		hex.set((char*)keyblob, size);
		s = hex.toString();

		RegKeyWriter reg;
		LONG rc;
		rc = reg.SetValue(hKeyParent, lpszKeyName, s.c_str(), lpszValueName);
		delete [] keyblob;
		if (rc != ERROR_SUCCESS)
			throw crypt_exception(rc);
		return true;
	}

	inline bool TripleDesKey::loadKey(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName)
	{
		BYTE* keyblob = 0;

		RegKeyReader reg;
		LONG rc;
		rc = reg.Open(hKeyParent, lpszKeyName);
		char* szvalue = new char[512];
		DWORD dwsize = 512;
		rc = reg.QueryValue(szvalue, lpszValueName, &dwsize);
		if (rc != ERROR_SUCCESS) {
			if (dwsize <= 512) {
				delete [] szvalue;
				throw crypt_exception(rc);
			}
			else {
				delete [] szvalue;
				szvalue = new char[dwsize];
				rc = reg.QueryValue(szvalue, lpszValueName, &dwsize);
				if (rc != ERROR_SUCCESS) {
					delete [] szvalue;
					throw crypt_exception(rc);
				}
			}
		}
		DWORD len;
		
		std::string s(szvalue);
		Hexer hex(s);
		len = hex.getlength();
		keyblob = new BYTE[len];
		hex.copyContent((char*)keyblob, len);

		delete [] szvalue;

		HCRYPTKEY ukey;
		if (CryptGetUserKey(*p_ctx, AT_KEYEXCHANGE, &ukey) == FALSE) {
			delete [] keyblob;
			throw crypt_exception();
		}
		HCRYPTKEY tkey = 0;
		if (CryptImportKey(*p_ctx, keyblob, len, ukey, CRYPT_EXPORTABLE, &tkey) == FALSE) {
			delete [] keyblob;
			crypt_exception e;
			CryptDestroyKey(ukey);
			throw e;
		}
		delete [] keyblob;
		CryptDestroyKey(ukey);
		CryptDestroyKey(m_key);
		m_key = tkey;
		return true;
	}
#endif	// __KEY_BLOB_IN_REGISTRY__
}

#endif __CRYPTO_H__