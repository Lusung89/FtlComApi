
//TsccDongleApi.h
#include <tchar.h>

#define EDCSIGFILE   "c:\\Ftlinline\\dll\\dat\\EdcSig.gz"
#define EDCSIGFILETMP   "c:\\Ftlinline\\dll\\dat\\EdcSig.tmp"
#define TRANSEDCSIGFILE   "c:\\Ftlinline\\dll\\dat\\TransEdcSig.gz"
#define EDCTRACEDIR       "c:\\Ftlinline\\log\\"

#define SUCCEED         0
#define FAILURE        -1
#define EDCTIMEOUT     -99

#define STX     0x02
#define ETX     0x03
#define ACK     0x06
#define NAK     0x15
#define CR      0x0D
#define LF      0x0A
#define ESC     0x1B

//#define INLINE_WORK_PATH   "c:\\abacus\\"
//#define MWR_SETLOCALTIME       WM_APP+3153

// #define MAX_PAGE_REC  9
//typedef unsigned char   uchar;
//typedef void __fastcall (__closure *TNotifyEventError)(System::TObject* Sender, AnsiString sStrMsg, int iErrNo);

/**                     �S�O�`�N TCHAR=world : 2Byte  , �n�� char  || CHAR                  ****/


typedef struct //���Ⱦ��_�l�T���榡 HWEDC_Data_0x30;
{
  CHAR    field_01[1];       // �T���N�X        �T�w�� CHAR(30)
  CHAR    field_02[6];       // ����            �k�a���ɹs
  CHAR    field_03[8];       // ���Ⱦ��N��      �k�a���ɹs
  CHAR    field_04[8];       // ���Ⱦ�����Ǹ�
  CHAR    field_05[6];       // ���ȭ��N�X      ���a�k�ɪť�
  CHAR    field_06[4];       // ����N�X
  CHAR    field_07[12];      // ������B        ��� 10 , �p�� 2
  CHAR    field_08[12];      // �o�����X        2 ��ť� + 10��o�����X
  CHAR    field_09[8];       // ��d���ݥ��N��  �k�a���ɹs
  CHAR    field_10[15];      // ��d���ө��N��  �k�a���ɹs
  CHAR    field_11[3];       // ��d�� Timeout ��
  CHAR    field_12[8];       // ����Ǹ� / ��d���վ\�s�� �k�a���ɹs
  CHAR    field_13[12];      // ����ѦҸ��X(�q��s��)
  CHAR    field_14[30];      // �d���d��   �ɪť�
  CHAR    field_15[1];       // ��d�����A �ɪť�
  CHAR    field_salereq[30]; // �~�ȻݨD���
  CHAR    field_specil[100]; // �S�O���
} HWEDC_Data_0x30; //�`���� 234


typedef struct //���Ⱦ��_�l�T���榡 "�~�ȻݨD���" �i�}�� 1
{
  CHAR    field_01[2];       // �~�ȥN�� "01"
  CHAR    field_02[8];       // ���Ⱦ��J�b��� yyyymmdd
  CHAR    field_03[20];      // �O�d��� ��ť�
} HWEDC_Data_0x30_salreq1; //�`���� 30


typedef struct //���Ⱦ��_�l�T���榡 "�~�ȻݨD���" �i�}�� 2
{
  CHAR    field_01[2];       // �~�ȥN�� "10"
  CHAR    field_02[20];      // �̹�b�ݨD�U�۩w�q
  CHAR    field_03[8];       // �O�d��� ��ť�
} HWEDC_Data_0x30_salreq2; //�`���� 30


typedef struct //���Ⱦ��_�l�T���榡 "�S�O���" �i�}�� 1 (�o�P�d���O���)
{
  CHAR    field_01[12];      // �覩����B      ��� 10 , �p�� 2
  CHAR    field_02[6];       // �o�~�ƶq        ��� 4 , �p�� 2
  CHAR    field_03[10];      // �o�~���O        ���a�k�ɪť�
  CHAR    field_04[10];      // ����            ���a�k�ɪť�
  CHAR    field_05[6];       // ���{��          �k�a���ɹs
  CHAR    field_06[6];       // �Ȥ����        ���a�k�ɪť�
  CHAR    field_07[20];      // �O�d���        �ť�
} HWEDC_Data_0x30_spec1; //�`���� 70


typedef struct //���Ⱦ��_�l�T���榡 "�S�O���" �i�}�� 2 (���b���)
{
  CHAR    field_01[1];       // ���`����C�L    'Y'=�[�L , 'N'=���[�L
  CHAR    field_02[69];      // �O�d���        �ť�
} HWEDC_Data_0x30_spec2; //�`���� 70

//---------------------------------------------------------------------------
/**   �S�O�`�N TCHAR : Byte
typedef struct //��d���_�l�T���榡 HWEDC_Data_0x31;
{
  TCHAR    field_01[1];       // �T���N�X        �T�w�� CHAR(31)
  TCHAR    field_02[6];       // ����            �k�a���ɹs
  TCHAR    field_03[8];       // ���Ⱦ��N��      �k�a���ɹs
  TCHAR    field_04[8];       // ���Ⱦ�����Ǹ�
  TCHAR    field_05[6];       // ���ȭ��N�X      ���a�k�ɪť�
  TCHAR    field_06[4];       // ����N�X
  TCHAR    field_07[12];      // ������B        ��� 10 , �p�� 2
  TCHAR    field_08[12];      // �o�����X
  TCHAR    field_09[8];       // ��d���ݥ��N��  �k�a���ɹs
  TCHAR    field_10[15];      // ��d���ө��N��  �k�a���ɹs
  TCHAR    field_11[3];       // ��d�� Timeout  ��
  TCHAR    field_12[8];       // ����Ǹ�        �k�a���ɹs
  TCHAR    field_13[12];      // ����ѦҸ��X    (�q��s��)
  TCHAR    field_14[30];      // �d��            ���a�k�ɪť�
  TCHAR    field_15[1];       // ��d�����A      '0'=�d���w���J , '1'=�L�d��,��d�����ݤ�
  TCHAR    field_16[30];      // �~�ȻݨD���
  TCHAR    field_specil[70];  // �S�O���
} HWEDC_Data_0x31;  //�`���� 468
**/

typedef struct //��d���_�l�T���榡 HWEDC_Data_0x31;
{
    char    field_01[1];       // �T���N�X        �T�w�� CHAR(31)
    char    field_02[6];       // ����            �k�a���ɹs
    char    field_03[8];       // ���Ⱦ��N��      �k�a���ɹs
    char    field_04[8];       // ���Ⱦ�����Ǹ�
    char    field_05[6];       // ���ȭ��N�X      ���a�k�ɪť�
    char    field_06[4];       // ����N�X
    char    field_07[12];      // ������B        ��� 10 , �p�� 2
    char    field_08[12];      // �o�����X
    char    field_09[8];       // ��d���ݥ��N��  �k�a���ɹs
    char    field_10[15];      // ��d���ө��N��  �k�a���ɹs
    char    field_11[3];       // ��d�� Timeout  ��
    char    field_12[8];       // ����Ǹ�        �k�a���ɹs
    char    field_13[12];      // ����ѦҸ��X    (�q��s��)
    char    field_14[30];      // �d��            ���a�k�ɪť�
    char    field_15[1];       // ��d�����A      '0'=�d���w���J , '1'=�L�d��,��d�����ݤ�
    char    field_16[30];      // �~�ȻݨD���
    char    field_specil[70];  // �S�O���
} HWEDC_Data_0x31;  //�`���� 234



typedef struct //���Ⱦ��_�l�T���榡 "�~�ȻݨD���" �i�}�� 1
{
  CHAR    field_01[2];       // �~�ȥN�� "01"
  CHAR    field_02[8];       // ���Ⱦ��J�b��� yyyymmdd
  CHAR    field_03[20];      // �O�d��� ��ť�
} HWEDC_Data_0x31_salreq1; //�`���� 30


typedef struct //���Ⱦ��_�l�T���榡 "�~�ȻݨD���" �i�}�� 2
{
  CHAR    field_01[2];       // �~�ȥN�� "10"
  CHAR    field_02[20];      // �̹�b�ݨD�U�۩w�q
  CHAR    field_03[8];       // �O�d��� ��ť�
} HWEDC_Data_0x31_salreq2; //�`���� 30


typedef struct //��d���_�l�T���榡�S�O���i�}�� 1 (�o�P�d���O���)
{
  CHAR    field_01[12];      // �覩����B      ��� 10 , �p�� 2
  CHAR    field_02[6];       // �o�~�ƶq        ��� 4 , �p�� 2
  CHAR    field_03[10];      // �o�~���O        ���a�k�ɪť�
  CHAR    field_04[10];      // ����            ���a�k�ɪť�
  CHAR    field_05[6];       // ���{��          �k�a���ɹs
  CHAR    field_06[6];       // �Ȥ����        ���a�k�ɪť�
  CHAR    field_07[20];      // �O�d���        �ť�
} HWEDC_Data_0x31_spec1; //�`���� 70


typedef struct //��d���_�l�T���榡�S�O���i�}�� 2 (���b���)
{
  TCHAR    field_01[1];       // ���`����C�L    'Y'=�[�L , 'N'=���[�L
  TCHAR    field_02[69];      // �O�d���        �ť�
} HWEDC_Data_0x31_spec2; //�`���� 70


//---------------------------------------------------------------------------
typedef struct //��d������^���T���榡�S�O���i�}�� for FETC X50
{
  CHAR    field_01[1];       // �d�����O        ���a�k�ɪť�
  CHAR    field_02[20];      // �d���d��
  CHAR    field_03[8];       // ���Ĥ��
  CHAR    field_04[6];       // ��d���վ\�s��  �k�a���ɹs
  CHAR    field_05[6];       // ��d���妸��
  CHAR    field_06[10];      // ��ڥ�����B    ��l������B    ��� 10 , //�p�� 2
  CHAR    field_07[10];      // �����l�B      ��ڥ�����B    ��� 10 , //�p�� 2
  CHAR    field_08[12];      // ����ѦҸ��X
  CHAR    field_09[6];       // ������v�X
  CHAR    field_10[10];      // ����e�l�B      ��ڥ�����B    ��� 10 , //�p�� 2
  CHAR    field_11[8];       // ���ҥd�s��
  CHAR    field_12[10];      // �d���Ǹ�
  CHAR    field_13[8];       // �d������Ǹ�
  CHAR    field_14[8];       // �d������ɶ�
  CHAR    field_15[8];       // �d�����ڧǸ�
  CHAR    field_16[8];       // �[�Ⱦ����ҽs��
  CHAR    field_17[6];       // �[�Ⱦ����ҧǸ�
  CHAR    field_18[8];       // �������ҽs��
  CHAR    field_19[8];       // �������ҧǸ�
  CHAR    field_20[16];      // ���ڶü����ҽX
  CHAR    field_21[16];      // ���q�[�����ҽX
  CHAR    field_22[16];      // IC�d�������ҽX
  CHAR    field_23[1];       // Y ��ܦ������AUTOLOAD
  CHAR    field_24[16];      // �O�d���        �ť�
} HWEDC_Data_0x50_FETCspec;  // �`���� 226


typedef struct //��d������^���T���榡�S�O���i�}�� for TSCC X50
{
  CHAR    field_01[1];       // �d�����O        ���a�k�ɪť�
  CHAR    field_02[20];      // �d���d��
  CHAR    field_03[8];       // ���Ĥ��
  CHAR    field_04[6];       // ��d���վ\�s��  �k�a���ɹs
  CHAR    field_05[6];       // ��d���妸��
  CHAR    field_06[10];      // ��ڥ�����B    ��l������B    ��� 10 , //�p�� 2
  CHAR    field_07[10];      // �����l�B      ��ڥ�����B    ��� 10 , //�p�� 2
  CHAR    field_08[12];      // ����ѦҸ��X
  CHAR    field_09[6];       // ������v�X
  CHAR    field_10[10];      // ����e�l�B      ��ڥ�����B    ��� 10 , //�p�� 2
  CHAR    field_11[8];       // ����e�֭p�[��  ��ڥ�����B    ��� 8 , //�p�� 2
  CHAR    field_12[8];       // ����e�֭p����  ��ڥ�����B    ��� 8 , //�p�� 2
  CHAR    field_13[8];       // ���ҥd�s��
  CHAR    field_14[10];      // �d���Ǹ�
  CHAR    field_15[8];       // �d������Ǹ�
  CHAR    field_16[8];       // ����e�d�����ڧǸ�
  CHAR    field_17[8];       // �������ҧǸ�
  CHAR    field_18[16];      // ���ڶü����ҽX
  CHAR    field_19[16];      // ����/�[�����ҽX
  CHAR    field_20[8];       // ����e�d���[�ȧǸ�
  CHAR    field_21[8];       // �o�d�Ȧ�N��
  CHAR    field_22[1];       // Y ��ܦ������AUTOLOAD
  CHAR    field_23[30];      // �O�d���        �ť�
} HWEDC_Data_0x50_TSCCspec;  // �`���� 226


typedef struct //��d������^���T���榡 HWEDC_Data_0x50;
{
  CHAR    field_01[1];       // �T���N�X        �T�w�� CHAR(40)
  CHAR    field_02[6];       // ����            �k�a���ɹs
  CHAR    field_03[8];       // ���Ⱦ��N��      �k�a���ɹs
  CHAR    field_04[8];       // ���Ⱦ�����Ǹ�
  CHAR    field_05[6];       // ���ȭ��N�X      ���a�k�ɪť�
  CHAR    field_06[4];       // ����N�X
  CHAR    field_07[4];       // �^���N�X
  CHAR    field_08[8];       // ��d���ݥ��N��  �k�a���ɹs
  CHAR    field_09[15];      // ��d���ө��N��  �k�a���ɹs
  CHAR    field_10[8];       // ������
  CHAR    field_11[6];       // ����ɶ�   74 byte
  union
     {
     TCHAR    field_specil[226]; // �S�O���
     HWEDC_Data_0x50_FETCspec fetc;
     HWEDC_Data_0x50_TSCCspec tscc;
     }u;
} HWEDC_Data_0x50;  //�`���� 300




typedef struct //��d������^���T���榡 HWEDC_Data_0x40;
{
  CHAR    field_01[1];       // �T���N�X        �T�w�� CHAR(40)
  CHAR    field_02[6];       // ����            �k�a���ɹs
  CHAR    field_03[8];       // ���Ⱦ��N��      �k�a���ɹs
  CHAR    field_04[8];       // ���Ⱦ�����Ǹ�
  CHAR    field_05[6];       // ���ȭ��N�X      ���a�k�ɪť�
  CHAR    field_06[4];       // ����N�X
  CHAR    field_07[4];       // �^���N�X
  CHAR    field_08[8];       // ��d���ݥ��N��  �k�a���ɹs
  CHAR    field_09[15];      // ��d���ө��N��  �k�a���ɹs
  CHAR    field_10[8];       // ������
  CHAR    field_11[6];       // ����ɶ�
  CHAR    field_specil[152]; // �S�O���
  CHAR    field_speci2[50];  // �q�l�o������   2018/01/22   Lu update
  CHAR    field_speci3[124];  // �O�d���       2018/01/22   Lu update
} HWEDC_Data_0x40;    //�`���� 226 + 50 + 124 = 400 


typedef struct //��d������^���T���榡�S�O���i�}�� 1 (�H�Υd/AE�d/�j�ӥd : ���O,����)
{
  CHAR    field_01[30];      // �d��            ���a�k�ɪť�
  CHAR    field_02[12];      // ����ѦҸ��X
  CHAR    field_03[6];       // ����վ\�s��    �k�a���ɹs
  CHAR    field_04[6];       // ��d���妸��    �k�a���ɹs
  CHAR    field_05[6];       // ������v�X
  CHAR    field_06[12];      // ��l������B    ��� 10 , �p�� 2
  CHAR    field_07[12];      // ��ڥ�����B    ��� 10 , �p�� 2
  CHAR    field_08[9];       // ��������I��    �k�a���ɹs
  CHAR    field_09[12];      // ���������B    ��� 10 , �p�� 2
  CHAR    field_10[9];       // �Ѿl�I��        �k�a���ɹs
  CHAR    field_11[1];       // Ū�d�覡
  CHAR    field_12[1];       // ñ�W
  CHAR    field_13[1];       // �d�����O
  CHAR    field_14[10];      // �d�����O�W��
  CHAR    field_15[25];      // �O�d���        �ť�
} HWEDC_Data_0x40_spec1;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 2 (�o�P�d : ���O,����)
{
  CHAR    field_01[30];      // �d��            ���a�k�ɪť�
  CHAR    field_02[12];      // ����ѦҸ��X
  CHAR    field_03[6];       // ����վ\�s��    �k�a���ɹs
  CHAR    field_04[6];       // ��d���妸��    �k�a���ɹs
  CHAR    field_05[6];       // ������v�X
  CHAR    field_06[12];      // ��l������B    ��� 10 , �p�� 2
  CHAR    field_07[12];      // ��ڥ�����B    ��� 10 , �p�� 2
  CHAR    field_08[1];       // �o�P�d�O
  CHAR    field_09[6];       // �o�~�ƶq        ��� 4 , �p�� 2
  CHAR    field_10[10];      // �o�~���O        ���a�k�ɪť�
  CHAR    field_11[10];      // ����            ���a�k�ɪť�
  CHAR    field_12[6];       // ���{��          �k�a���ɹs
  CHAR    field_13[35];      // �O�d���        �ť�
} HWEDC_Data_0x40_spec2;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 3 (SVC��� : ���O,���O����,�[��,�[�Ȩ���)
{
  CHAR    field_01[30];      // �d��            ���a�k�ɪť�
  CHAR    field_02[12];      // ����ѦҸ��X
  CHAR    field_03[6];       // ����վ\�s��    �k�a���ɹs
  CHAR    field_04[6];       // ��d���妸��    �k�a���ɹs
  CHAR    field_05[6];       // ������v�X
  CHAR    field_06[12];      // ��l������B    ��� 10 , �p�� 2
  CHAR    field_07[12];      // ��ڥ�����B    ��� 10 , �p�� 2
  CHAR    field_08[8];       // �d������Ǹ�    �k�a���ɹs
  CHAR    field_09[8];       // �d�����ڧǸ�    �k�a���ɹs
  CHAR    field_10[8];       // �d���[�ȧǸ�    �k�a���ɹs
  CHAR    field_11[1];       // AutoLoad        "Y" �� "N"
  CHAR    field_12[12];      // AutoLoad ���B   ��� 10 , �p�� 2
  CHAR    field_13[8];       // �{���I�l�B      �k�a���ɹs
  CHAR    field_14[8];       // ���Q�I�l�B      �k�a���ɹs
  CHAR    field_15[1];       // �d�����O        "1"=�Ȧ�d "2"=���q�d
  CHAR    field_16[14];      // �O�d���        �ť�
} WEDC_Data_0x40_spec3;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 4 (�H�Υd/AE�d/�j�ӥd/�o�P�d : ���b,�`�b�d��)
{
  CHAR    field_01[6];       // �妸��          �k�a���ɹs
  CHAR    field_02[6];       // �P���`����      �k�a���ɹs
  CHAR    field_03[12];      // �P���`���B      �k�a���ɹs
  CHAR    field_04[6];       // �h�f�`����      �k�a���ɹs
  CHAR    field_05[12];      // �h�f�`���B      �k�a���ɹs
  CHAR    field_06[110];     // �O�d���        �ť�
} HWEDC_Data_0x40_spec4;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 5 (SVC��� : ���b,�`�b�d��)
{
  CHAR    field_01[6];       // �妸��          �k�a���ɹs
  CHAR    field_02[6];       // �P���`����      �k�a���ɹs
  CHAR    field_03[12];      // �P���`���B      �k�a���ɹs
  CHAR    field_04[6];       // �h�f�`����      �k�a���ɹs
  CHAR    field_05[12];      // �h�f�`���B      �k�a���ɹs
  CHAR    field_06[6];       // �H�Υ[���`����  �k�a���ɹs
  CHAR    field_07[12];      // �H�Υ[���`���B  �k�a���ɹs
  CHAR    field_08[6];       // �{���[���`����  �k�a���ɹs
  CHAR    field_09[12];      // �{���[���`���B  �k�a���ɹs
  CHAR    field_10[74];      // �O�d���        �ť�
} HWEDC_Data_0x40_spec5;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 6 (SVC��� : �l�B�d��)
{
  CHAR    field_01[30];      // �d��            ���a�k�ɪť�
  CHAR    field_02[8];       // �{���I�l�B      �k�a���ɹs
  CHAR    field_03[8];       // ���Q�I�l�B      �k�a���ɹs
  CHAR    field_04[1];       // �d�����O
  CHAR    field_05[105];     // �O�d���        �ť�
} HWEDC_Data_0x40_spec6;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 7 (���`����/�`�b/���� �C�L)
{
  CHAR    field_01[152];     // �O�d���        �ť�
} HWEDC_Data_0x40_spec7;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 8 (���`����/�`�b/���� �C�L)
{
  CHAR    field_01[30];      // �d��            ���a�k�ɪť�
  CHAR    field_02[122];     // �O�d���        �ť�
} HWEDC_Data_0x40_spec8;  //�`���� 152


typedef struct //��d������^���T���榡�S�O���i�}�� 9 (�H�Υd+SVC ���b)
{
  CHAR    field_01[6];       // �妸��          �k�a���ɹs (�H�Υd)
  CHAR    field_02[6];       // �P���`����      �k�a���ɹs (�H�Υd)
  CHAR    field_03[12];      // �P���`���B      �k�a���ɹs (�H�Υd)
  CHAR    field_04[6];       // �h�f�`����      �k�a���ɹs (�H�Υd)
  CHAR    field_05[12];      // �h�f�`���B      �k�a���ɹs (�H�Υd)
  CHAR    field_06[8];       // ��d���ݥ��N��  �k�a���ɹs (SVC)
  CHAR    field_07[15];      // ��d���ө��N��  �k�a���ɹs (SVC)
  CHAR    field_08[6];       // ��d���妸��    �k�a���ɹs (SVC)
  CHAR    field_09[6];       // �P���`����      �k�a���ɹs (SVC)
  CHAR    field_10[12];      // �P���`���B      �k�a���ɹs (SVC)
  CHAR    field_11[6];       // �h�f�`����      �k�a���ɹs (SVC)
  CHAR    field_12[12];      // �h�f�`���B      �k�a���ɹs (SVC)
  CHAR    field_13[6];       // �H�Υ[���`����  �k�a���ɹs (SVC)
  CHAR    field_14[12];      // �H�Υ[���`���B  �k�a���ɹs (SVC)
  CHAR    field_15[6];       // �{���[���`����  �k�a���ɹs (SVC)
  CHAR    field_16[12];      // �{���[���`���B  �k�a���ɹs (SVC)
  CHAR    field_17[9];       // �O�d���        �ť�
} HWEDC_Data_0x40_spec9;  //�`���� 152


/////////////////////////////////////    2016/01/12 Update     ///////////////////////////////////////////

typedef struct //���p�d��� : ���O,�h�f   2010,2030
{
  CHAR    field_01[30];      // �d��            ���a�k�ɪť�
  CHAR    field_02[12];      // ����ѦҸ��X
  CHAR    field_03[6];       // ����վ\�s��    �k�a���ɹs
  CHAR    field_04[6];       // ��d���妸��    �k�a���ɹs
  CHAR    field_05[6];       // ������v�X
  CHAR    field_06[12];      // ��l������B    ��� 10 , �p�� 2
  CHAR    field_07[12];      // ��ڥ�����B    ��� 10 , �p�� 2
  CHAR    field_08[9];       // ��������I��    �k�a���ɹs
  CHAR    field_09[12];      // ���������B    �k�a���ɹs
  CHAR    field_10[9];       // �Ѿl�I��        �k�a���ɹs
  CHAR    field_11[1];       // ���Ū�d�覡    "S,I,W"
  CHAR    field_12[1];       // ñ��ñ�W
  CHAR    field_13[6];       // ���p�d����Ǹ�  �k�a���ɹs
  CHAR    field_14[4];       // ���p�d������  �k�a���ɹs
  CHAR    field_15[4];       // ���p�d������
  CHAR    field_16[6];       // ���p�d����ɶ�
  CHAR    field_17[16];      // �O�d���        �ť�

} HWEDC_Data_0x40_spec10;  //�`���� 152



typedef struct //�������ĥd : ���O,�h�f  3010,3030
{
  CHAR    field_01[30];      // �d��            ���a�k�ɪť�
  CHAR    field_02[12];      // ����ѦҸ��X
  CHAR    field_03[6];       // ����վ\�s��    �k�a���ɹs
  CHAR    field_04[6];       // ��d���妸��    �k�a���ɹs
  CHAR    field_05[6];       // ������v�X
  CHAR    field_06[12];      // ��l������B    ��� 10 , �p�� 2
  CHAR    field_07[12];      // ��ڥ�����B    ��� 10 , �p�� 2
  CHAR    field_08[8];       // ��������I��    �k�a���ɹs
  CHAR    field_09[1];       // ���������B    �k�a���ɹs
  CHAR    field_10[1];       // �Ѿl�I��        �k�a���ɹs
  CHAR    field_11[66];       // �O�d���        �ť�

} HWEDC_Data_0x40_spec11;  //�`���� 152


typedef struct
{
  CHAR cTotalCount[2];
  CHAR cSendCount[2];
  CHAR cReceiveCount[2];

}  DownLoadInfo;


typedef struct
{
  CHAR  uMsgid[1];
  CHAR  cStatus[2];
  CHAR  cLen[2];
  CHAR  cErmType[1];
  CHAR  cFileSize[2];
  CHAR  cCompressFlag[1];
  DownLoadInfo sDownLoadInfo;
  CHAR cBlockLen[2];
  CHAR uBlockData[4096];

}  HWEDC_Data_0x42;


/****
typedef struct
{
  unsigned short int  iTotalCount;
  unsigned short int  iSendCount;
  unsigned short int  iReceiveCount;

}  DownLoadInfo;


typedef struct
{
  uchar uMsgid[1];
  unsigned short int  iStatus;
  unsigned short int  iLen;
  CHAR cErmType[1];
  unsigned short int  iFileSize;
  CHAR cCompressFlag[1];
  DownLoadInfo sDownLoadInfo;
  unsigned short int  iBlockLen;
  uchar uBlockData[4096];

}  HWEDC_Data_0x42;

*****/




/*
 *-----------------------------------------------------------------------------
 * End of Include file
 *-----------------------------------------------------------------------------
 */
