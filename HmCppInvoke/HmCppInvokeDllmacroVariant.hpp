/*
 * Copyright (c) 2021 Akitsugu Komiyama
 * under the MIT License
 */


class TDllMacroVariant {
public:
    TDllMacroVariant();

public:
    // dll�̌Ă΂ꂩ���A�֐��̌Ă΂��(�Ԃ�l�̗v���^������̗v���^�𔻒�ł���֐�)�B����
    using PFNGetDllFuncCalledType = int(WINAPI*)(int n);
    static PFNGetDllFuncCalledType Hidemaru_GetDllFuncCalledType;

public:
    //-------------------------------------------------------------------------
    // n�� - 1���w�肷��ƁAloaddll�̂������Ԃ��܂��B1�ȏ��loaddll�֐��̕Ԃ�l�Ɠ����ł��B - 1�̏ꍇ��loaddll���œǂݍ��܂ꂽDLL�ł��B

    //-------------------------------------------------------------------------
    // n��0���w�肷��ƁAdllfunc / dllfuncw / dllfuncstr / dllfuncstrw�̂ǂ�ŌĂ΂ꂽ���ɂ���āAreturn�ŕԂ��ׂ��l�������܂��B
    // �ȉ��̒l�̂����ꂩ���Ԃ�܂��B
    // dllfunc�����Ă΂�Ă���Ƃ��A�ǂ̂悤�ȌĂ΂�������Ă��邩���擾���܂��B
    enum class DLLFUNCRETURN {
        NOTCALL = 0,       //�Ă΂�Ă��Ȃ�
        INT = 0x0001,  //int��Ԃ��ׂ�(dllfunc�܂���dllfuncw)
        CHAR_PTR = 0x0002,  //char*��Ԃ��ׂ�(dllfuncstr)
        WCHAR_PTR = 0x0003,  //WCHAR*��Ԃ��ׂ�(dllfuncstrw)
        DOUBLE = 0x0004  //double��Ԃ��ׂ�(dllfunc�܂���dllfuncw�ŕ��������_�����L��)
    };

    //-------------------------------------------------------------------------
    //n��1���琔�������Ԃ̒l���w�肷��ƁA1���琔�����p�����[�^�̎�ނ����l / ������ / Unicode������ / ���������_������Ԃ��܂��B
    // �p�����[�^�̍ő��64�ł��B
    // �ȉ��̒l�̂����ꂩ���Ԃ�܂��B
    enum class DLLFUNCPARAM {
        NOPARAM = 0,       //�ȍ~�̃p�����[�^�Ȃ�
        INT = 0x0100,  //int�̃p�����[�^
        CHAR_PTR = 0x0200,  //char*�̃p�����[�^
        WCHAR_PTR = 0x0300,  //WCHAR*�̃p�����[�^
        DOUBLE = 0x0400   //double�̃p�����[�^
    };
};