

/***************************************************************/
//Author : Rainbow
/***************************************************************/

#ifndef _LLIB_BASIC_CONFIG_H
#define _LLIB_BASIC_CONFIG_H


#ifdef _MSC_VER							//Windows
#	define __LLIB_USE_EXCEPTION								//ʹ���쳣����
//#	define __LLIB_USE_ASSERTION								//ʹ��Assert
#	define __LLIB_USE_ASSERTION_INSTEAD_OF_EXCEPTION		//��ҪThrow�ĵط�ֱ��Assert����Ҫ����Assert
#	define __LLIB_USE_NAMESPACE								//ʹ�������ռ�
//#	define __LLIB_USE_RTTI									//ʹ�ñ�������dynamic_cast��
#	define __LLIB_USE_INTERNAL_RTTI							//ͨ����ÿ�����и�����Ϣ���ﵽRTTI��Ч������ֹ������RTTI
#	define __WIN32

#elif defined(_ROS)	//�ҵĲ���ϵͳ
#	define __LLIB_USE_INTERNAL_RTTI							//ͨ����ÿ�����и�����Ϣ���ﵽRTTI��Ч��
#	define __LLIB_OVERLOAD_NEW_AND_DELETE					//����new��delete������

#elif defined(__LINUX)					//GCC
#	define __LLIB_USE_EXCEPTION								//ʹ���쳣����
//#	define __LLIB_USE_ASSERTION								//ʹ��Assert
#	define __LLIB_USE_ASSERTION_INSTEAD_OF_EXCEPTION		//��ҪThrow�ĵط�ֱ��Assert����Ҫ����Assert
#	define __LLIB_USE_NAMESPACE								//ʹ�������ռ�
//#	define __LLIB_USE_RTTI									//ʹ�ñ�������dynamic_cast��
#	define __LLIB_USE_INTERNAL_RTTI							//ͨ����ÿ�����и�����Ϣ���ﵽRTTI��Ч������ֹ������RTTI

#elif defined(__CROS)

#	define __LLIB_USE_NAMESPACE								//ʹ�������ռ�
#	define __LLIB_USE_INTERNAL_RTTI							//ͨ����ÿ�����и�����Ϣ���ﵽRTTI��Ч������ֹ������RTTI

#endif


//�������Բ��Ժ������ʵ�����


//�Ƿ�ʹ��RTTI

#ifdef __LLIB_USE_RTTI
#	define __LLIB_DYNAMIC_CAST(type,obj)		dynamic_cast<type>(obj)
#	define __LLIB_TYPE_ID(type,obj)				typeid<type>(obj)
#else
#	define __LLIB_DYNAMIC_CAST(type,obj)		(type)obj		//C���ǿ������ת��
#	define __LLIB_TYPE_ID(type,obj)				
#endif

//ʹ������RTTI
#ifdef __LLIB_USE_INTERNAL_RTTI
#	include"typeinfo.h"
#	define __LLIB_CLASS_DECLARE(classname,father)		public:\
														public:\
																static Typeinfo __llib_static_get_typeinfo()\
																{\
																return Typeinfo(#classname,	\
																		father::__llib_static_get_typeinfo);\
																}\
																virtual Typeinfo (*__llib_get_typeinfo())()\
																{\
																	return this->__llib_static_get_typeinfo; \
																}\
														private:
#	undef __LLIB_DYNAMIC_CAST
#	undef __LLIB_TYPE_ID	
#	undef __LLIB_TYPE_ID_USE_VAR
#	define __LLIB_DYNAMIC_CAST(type,obj)				((type)obj)
#	define __LLIB_TYPE_ID_USE_TYPE(type)				type::__llib_static_get_typeinfo()	
#	define __LLIB_TYPE_ID_USE_VAR(var)					var.__llib_get_typeinfo()()	
#else
#	define __LLIB_CLASS_DECLARE(classname,father)
#endif

//ʹ�������ռ�
#ifdef __LLIB_USE_NAMESPACE
#	define __LLIB_NAMESPACE				lr
#	define __LLIB_BEGIN_NAMESPACE		namespace __LLIB_NAMESPACE{
#	define __LLIB_END_NAMESPACE			}
#else
#	define __LLIB_NAMESPACE				
#	define __LLIB_BEGIN_NAMESPACE		
#	define __LLIB_END_NAMESPACE			
#endif

//ʹ�ö��Ի���
#ifdef __LLIB_USE_ASSERTION
#	include<stdio.h>
#	define __LLIB_ASSERT(expr) if(!(expr)){fprintf(stderr,"\nFile:\t %s:\nLine:\t %d\nFunc:\t%s\n assertion failure:\t %s\n",__FILE__,__LINE__,__FUNCTION__,#expr);for(;;);}
#else
#	define __LLIB_ASSERT(expr) 
#endif


//ʹ���쳣����
#ifdef __LLIB_USE_EXCEPTION			
#	define __LLIB_RETHROW				throw
#	define __LLIB_THROW(...)			throw(__VA_ARGS__)
#	define __LLIB_CATCH_ALL				catch(...)
#	define __LLIB_CATCH(...)			catch(__VA_ARGS__)
#	define __LLIB_TRY					try
#	define __LLIB_UNWIND(action)		catch(...){action;throw;}
#else		//���쳣����
#	ifdef __LLIB_USE_ASSERTION_INSTEAD_OF_EXCEPTION		//ʹ�ö��Զ������쳣
#		define __LLIB_THROW(...)		__LLIB_ASSERT(false)					
#	else												//���������д���
#		define __LLIB_THROW(...)		
#	endif

#	define __LLIB_RETHROW				
#	define __LLIB_CATCH_ALL				if(false)
#	define __LLIB_CATCH(...)			if(false)
#	define __LLIB_TRY					if(true)
#	define __LLIB_UNWIND(action)		
#endif


//�������α���
__LLIB_BEGIN_NAMESPACE
typedef long long lrint64;
typedef unsigned  long long lruint64;
#ifdef _MSC_VER
typedef __int64 lrint64;
typedef unsigned __int64 lruint64;
#elif defined(__GUNC__)					//GCC
typedef long long lrint64;
typedef unsigned  long long lruint64;
#elif defined(_ROS)	//�ҵĲ���ϵͳ

#endif
__LLIB_END_NAMESPACE



#endif
