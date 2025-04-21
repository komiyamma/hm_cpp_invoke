/*
 * Copyright (c) 2021-2022 Akitsugu Komiyama
 * under the MIT License
 */

/// <summary>
/// �G�f�B�^�g
/// </summary>
class TEdit {
    // ���ݕҏW���̑S�Ẵe�L�X�g�𓾂�
    using PFNGetTotalTextUnicode = HGLOBAL(WINAPI*)(void);
    static PFNGetTotalTextUnicode Hidemaru_GetTotalTextUnicode;
    // ���ݕҏW���̑I���e�L�X�g�𓾂�(�P���I���̂�)
    using PFNGetSelectedTextUnicode = HGLOBAL(WINAPI*)(void);
    static PFNGetSelectedTextUnicode Hidemaru_GetSelectedTextUnicode;
    // ���ݕҏW���̃e�L�X�g�̃J�[�\��������s�̃e�L�X�g�𓾂�
    using PFNGetLineTextUnicode = HGLOBAL(WINAPI*)(int nLineNo);
    static PFNGetLineTextUnicode Hidemaru_GetLineTextUnicode;
    // ���ݕҏW���̃e�L�X�g�̃J�[�\���̈ʒu���擾����B�}�N����column��lineno����(�����ɂ͈قȂ�)
    using PFNGetCursorPosUnicode = BOOL(WINAPI*)(int* pnLineNo, int* pnColumn);
    static PFNGetCursorPosUnicode Hidemaru_GetCursorPosUnicode;
    // ���ݕҏW���̃e�L�X�g�̃}�E�X�̈ʒu�ɑΉ�����J�[�\���̈ʒu���擾����B
    using PFNGetCursorPosUnicodeFromMousePos = BOOL(WINAPI*)(POINT* ppt, int* pnLineNo, int* pnColumn);
    static PFNGetCursorPosUnicodeFromMousePos Hidemaru_GetCursorPosUnicodeFromMousePos;
    // �L�[���͂�����Ȃǂ̗��R�ŏ����𒆒f����ׂ�����Ԃ��܂��B
    using PFNCheckQueueStatus = BOOL(WINAPI*)();
    static PFNCheckQueueStatus Hidemaru_CheckQueueStatus;
    // �e��̓��͂��ł��邩�ǂ����𔻒f���邽�߂̏�Ԃ�\���܂��B
	using PFNGetInputStates = int(WINAPI*)();
	static PFNGetInputStates Hidemaru_GetInputStates;

public:
    TEdit();
public:
    /// <summary>
    /// ���݃A�N�e�B�u�ȕҏW�t�@�C������Ԃ��B����Ȃ�󕶎���Ԃ��B
    /// </summary>
    /// <returns>�t�@�C�����B����Ȃ�󕶎��B</returns>
    std::wstring getFilePath();

    /// <summary>
    /// �}�N����updatecount�����̒l��Ԃ��B
    /// </summary>
    /// <returns>updatecount�����̒l�B</returns>
    int getUpdateCount();

    /// <summary>
    /// ���݃A�N�e�B�u�ȕҏW�̈�̃e�L�X�g�S�̂�Ԃ��B
    /// </summary>
    /// <returns>�ҏW�̈�̃e�L�X�g�S��</returns>
    std::wstring getTotalText();

    /// <summary>
    /// ���݃A�N�e�B�u�ȕҏW�̈�̃e�L�X�g���e���w��̕�����Ƃ���B
    /// </summary>
    /// <param name = "text">�V���ȃe�L�X�g���e</param>
    /// <returns>����ւ��̐���</returns>
    bool setTotalText(std::wstring text);

    /// <summary>
    /// ���݁A�P���I�����Ă���ꍇ�A���̑I�𒆂̃e�L�X�g���e��Ԃ��B
    /// </summary>
    /// <returns>�I�𒆂̃e�L�X�g���e</returns>
    std::wstring getSelectedText();

    /// <summary>
    /// ���݁A�P���I�����Ă���ꍇ�A���̑I�𒆂̃e�L�X�g���e���w��̕�����Ƃ���B
    /// </summary>
    /// <param name = "text">�V���ȃe�L�X�g���e</param>
    /// <returns>����ւ��̐���</returns>
    bool setSelectedText(std::wstring text);

    /// <summary>
    /// ���݁A�J�[�\��������s(�G�f�B�^�I)�̃e�L�X�g���e��Ԃ��B
    /// </summary>
    /// <returns>�J�[�\��������s�̃e�L�X�g���e</returns>
    std::wstring getLineText();
 
    /// <summary>
    /// ���݁A�J�[�\��������s(�G�f�B�^�I)�̃e�L�X�g���e���w��̕�����Ƃ���B
    /// </summary>
    /// <param name = "text">�V���ȃe�L�X�g���e</param>
    /// <returns>����ւ��̐���</returns>
    bool setLineText(std::wstring text);

    /// <summary>
    /// �L�[���͂�����Ȃǂ̗��R�ŏ����𒆒f����ׂ�����Ԃ��B
    /// </summary>
    /// <returns>���f����ׂ��Ȃ�true�A�����łȂ����false</returns>
    bool isQueueStatus();

	/// <summary>
	/// �e��̓��͂��ł��邩�ǂ����𔻒f���邽�߂̏�Ԃ�\���܂��B
	/// </summary>
	/// <returns>�}�N����inputstates�Ɠ����l</returns>
	int getInputStates();

public:
    /// <summary>
    /// getCursorPos()�̕Ԃ�l�̌^�B
    /// </summary>
    class ICursorPos {
        int lineno;
        int column;
    public:
        /// <summary>
        /// Unicode�I�ɍl����LineNo�B
        /// </summary>
        /// <returns>Unicode�I�ɍl�����s�B0�s�ڂ���</returns>
        int getLineNo();

        /// <summary>
        /// Unicode�I�ɍl����Column�B
        /// </summary>
        /// <returns>Column�I�ɍl�������B0���ڂ���</returns>
        int getColumn();

        ICursorPos(int lineno, int column);
    };

    /// <summary>
    /// getMousePos()�̕Ԃ�l�̌^�B
    /// </summary>
    class IMousePos {
        int lineno;
        int column;
        int x;
        int y;
    public:
        /// <summary>
        /// Unicode�I�ɍl����LineNo�B
        /// </summary>
        /// <returns>Unicode�I�ɍl�����s�B0�s�ڂ���</returns>
        int getLineNo();

        /// <summary>
        /// Unicode�I�ɍl����Column�B
        /// </summary>
        /// <returns>Column�I�ɍl�������B0���ڂ���</returns>
        int getColumn();

        
        /// <summary>
        /// �uwin32api��GetCursorPos�v�œ�����x�Ɗ�{�������́B
        /// </summary>
        /// <returns>�}�E�X��x���W</returns>
        int getX();

        /// <summary>
        /// �uwin32api��GetCursorPos�v�œ�����y�Ɗ�{�������́B
        /// </summary>
        /// <returns>�}�E�X��y���W</returns>
        int getY();
        IMousePos(int lineno, int column, int x, int y);
    };

    /// <summary>
    /// ���j�R�[�h�̃G�f�B�^�I�Ȋ��Z�ł̃J�[�\���̈ʒu��Ԃ��B
    /// </summary>
    /// <returns>���j�R�[�h�̃G�f�B�^�I�Ȋ��Z�ł̃J�[�\���̈ʒu</returns>
    ICursorPos getCursorPos();

    /// <summary>
    /// ���j�R�[�h�̃G�f�B�^�I�Ȋ��Z�ł̃}�E�X�̈ʒu�ɑΉ�����J�[�\���̈ʒu��Ԃ��B
    /// </summary>
    /// <returns>���j�R�[�h�̃G�f�B�^�I�Ȋ��Z�ł̃}�E�X�̈ʒu�ɑΉ�����J�[�\���̈ʒu</returns>
    IMousePos getMousePos();
};

