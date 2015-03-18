#include "dynmenu_callback.h"
#include "constants.h"

#include "registratewin.h"

#include <maxscrpt/maxscrpt.h>
#include <time.h>
#include <imenuman.h> 
#include <imenus.h>

#include <QMessageBox>
#include <QDir>
#include <QGridLayout>

DynMenuCallback::DynMenuCallback(void)
{
}

DynMenuCallback::~DynMenuCallback(void)
{
    closeScriptsWindow();
}

void DynMenuCallback::MenuItemSelected( int itemId /*= 0 */ )
{
    HWND hMax = ::GetCOREInterface()->GetMAXHWnd();

	switch (itemId)
	{
	case IDC_YMENU_PLUGIN:		//�����
		{
            showScriptsWindow();
		};
		break;
	case IDC_YMENU_CREATER:	//ģ�ʹ�����
		{

		};
		break;
	case IDC_YMENU_MODIFIER://������Դ�޸���
		{
			
		};
		break;
	case IDC_YMENU_EXPORTER:	//����������
		{
			MessageBox( hMax, "��������", "����", 0 );
		};
		break;

	case IDC_YMENU_IMPORTER:	//������
		{
			MessageBox( hMax, "���빤��", "����", 0 );
		};
		break;
	case IDC_YMENU_TOOLBAR:	//������
		{
			MessageBox( hMax, "��ʾ������", "������", 0 );
		};
		break;
	case IDC_YMENU_SETTING:	//����
		{
			MessageBox( hMax, "��ʾ���öԻ���", "����", 0 );
		};
		break;
	case IDC_YMENU_HELP:	//��������
		{
			MessageBox( hMax, "��������", "����", 0 );
		};
		break;
    case IDC_YMENU_REG:	// ע��
        {
            RegistrateWin* regWin = new RegistrateWin(hMax);
            regWin->show();
        };
        break;
	case IDC_YMENU_ABOUT:	//��������
		{
			MessageBox( hMax, "��������", "����", 0 );
		};
		break;

	default:break;
    }
}

bool DynMenuCallback::showScriptsWindow()
{
    HWND hMax = ::GetCOREInterface()->GetMAXHWnd();
    RegistrateWin* regWin = new RegistrateWin(hMax);

    if( regWin->isExpiration() )
    {
        // �ѵ���
        if( !regWin->isRegistered() )
        {
            MessageBox( hMax, "��ʹ�õİ汾��δע�ḱ������ע���ʹ�á�", "��ʾ", 0 );
            regWin->show();
            return false;
        }
    }

    if( m_scriptsDll.isLoaded() )
    {
        dllScripts lib = (dllScripts)m_scriptsDll.resolve("showScriptsWin");
        if ( lib )
        {
            lib(hMax, CODE);
            return true;
        }
    }

    QString pluginpath = QDir::toNativeSeparators( QDir::currentPath() + "/plugins/SouthgisMax" );
    m_scriptsDll.setFileName( pluginpath + "/ScriptsWin.dll" );
    if(m_scriptsDll.load())
    {
        dllScripts lib = (dllScripts)m_scriptsDll.resolve("showScriptsWin");
        if ( lib )
        {
            lib(hMax, CODE);
            return true;
        }
        else
        {
            MessageBox( hMax, "resolve function failed.", "error", 0 );
        }
    }
    else
    {
        MessageBox( hMax, pluginpath.toLocal8Bit().data(), "library file dosen't exists", 0 );
    }
    return false;
}

bool DynMenuCallback::closeScriptsWindow()
{
    if( m_scriptsDll.isLoaded() )
    {
        HWND hMax = ::GetCOREInterface()->GetMAXHWnd();
        dllScripts lib = (dllScripts)m_scriptsDll.resolve("closeScriptsWin");
        if ( lib )
        {
            lib(hMax, CODE);
            return true;
        }
    }
    m_scriptsDll.unload();
    return false;
}

