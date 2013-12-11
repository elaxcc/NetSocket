
#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <netinet/in.h>
#include <sys/time.h>
//#include <mysql.h>

#if !(WIN32 & WINNT)
#ifndef QT_VERSION
	typedef enum {FALSE = 0, TRUE} BOOL;
#endif
#endif

typedef enum {UNKNOWN = 0, FORWARD = 1, BACKWARD = 2} COURSE;

#if !(WIN32 & WINNT)
#define CONFIG_FILE				"/usr/local/etc/asood.conf"
#else
#define CONFIG_FILE				"c:/mps/asood.conf"
#endif

// Messages from controller
#define TEST_MAKING					((unsigned char) 0xF4)
#define RESET_MAKING				((unsigned char) 0xF6)
#define RESET_SUCCESS				((unsigned char) 0xF7)
#define FIRST_AXIS_FORWARD			((unsigned char) 0xF8)
#define FIRST_AXIS_BACKWARD			((unsigned char) 0xF9)
#define LAST_LOCO_AXIS				((unsigned char) 0xFA)
#define FIRST_AXIS					((unsigned char) 0xFB)
#define END_OF_TRAIN				((unsigned char) 0xFC)
#define END_OF_PACKET				((unsigned char) 0xFD)
#define SOFT_FAILURE				((unsigned char) 0xFE)
#define TEST_FAILED					((unsigned char) 0xFF)
#define TRAIN_STOPPED				((unsigned char) 0x00)

// Commands to controller
#define MAKE_FULL_TEST				((unsigned char) 0x55)
#define MAKE_TEST					((unsigned char) 0x66)
#define MAKE_RESET					((unsigned char) 0xE8)


#define LASER_BKTR_NUMBER			0
#define BOARD_BKTR_NUMBER			1
#define VIDEO_BKTR_NUMBER			2

#define VIDEO_IMAGE_SPOOL			"/var/spool/asood/jpeg"
#define VIDEO_MOVIE_SPOOL			"/var/spool/asood/movie"
#define VIDEO_CONVERTER				"/usr/local/bin/ffmpeg"
#define BOARD_IMAGE_SPOOL			"/var/spool/asood/board"

#define ASOOD_SERVER_PORT			9090
#define LASER_SERVER_PORT			9091
#define BOARD_SERVER_PORT			9092
#define VIDEO_SERVER_PORT			9093
#define SYNC_SERVER_PORT			9094

#define MYSQL_SERVER_IP				"127.0.0.1"
#define MYSQL_SERVER_PORT			3306
#define MYSQL_DB_NAME				"asood"
#define MYSQL_DB_USER				"asood"
#define MYSQL_DB_PASSWD				""

#define SOCKET_CONN_TIMEOUT			3 //seconds (�������� ����� ��������� �����������)

#define FIFO_LENGTH					100
typedef struct
{
	int r_index;
	int w_index;
	void *data[FIFO_LENGTH];
} FIFO;

#if !(WIN32 & WINNT)
	typedef struct
	{
		int left;
		int top;
		int right;
		int bottom;
	} RECT;
#endif

#define TITLE_TIME							11
#define TITLE_POSITION						12
#define TITLE_TR_UNIT_NUM					13
#define TITLE_DISTANCE						14

#define N_TITLE_MAX							4
#define MAX_TITLE_LEN						128
typedef struct
{
	int type;
	int x;
	int y;
	char text[MAX_TITLE_LEN];
} TITLE;

// type of events definitions
typedef enum
{
	EVENT_START_DATA_STREAM,			//00
	EVENT_STOP_DATA_STREAM,				//01
	EVENT_START_VIDEO_STREAM,			//02
	EVENT_STOP_VIDEO_STREAM,			//03
	EVENT_HEAD_OF_TRAIN,				//04
	EVENT_END_OF_TRAIN,					//05
	EVENT_LASER_JPEG,					//06
	EVENT_BOARD_JPEG,					//07
	EVENT_VIDEO_JPEG,					//08
	EVENT_CARRIAGE,						//09
	EVENT_AXIS,							//0A
	EVENT_DATA_AXIS_NUM,				//0B
	EVENT_DATA_SPEED,					//0C
	EVENT_DATA_STATE_WORD,				//0D
	EVENT_DATA_SENSOR_ERR,				//0E
	EVENT_DATA_END,						//0F
	EVENT_MEASURE_DATA,					//10
	EVENT_NEED_JPEG,					//11
	EVENT_BOARD_JPEG_INFO				//12
} CODE;
typedef struct
{
	int mysql_carriage_id;
	int axis_number;
} JPEG_INFO;

typedef struct
{
    CODE code;
#ifdef QT_VERSION
    struct
    {
        long tv_sec;
        long tv_usec;
    } timestamp;
#else
    struct timeval timestamp;
#endif
    int id;
    int value;
    int data_len;
} HEADER;

// �������� �������� ������������
typedef struct
{
	int fd;
	char host[30];
	int port;
#ifdef QT_VERSION
        bool connected;
#else
	BOOL connected;
#endif
	int timeout;
	struct pollfd *poll_entry;			// used when polling
	HEADER rcv_header;
#ifdef QT_VERSION
#else
	uint8_t *rcv_data;
#endif
	int rcv_bytes;
#ifdef QT_VERSION
#else
	uint8_t *snd_buffer;
#endif
	int snd_buffer_len;
} SERVER_CONTEXT;

#endif // _DEFINES_H_
