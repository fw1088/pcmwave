#include "drawpcm.h"
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QDebug>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
};

#define  OUTPUT_PCM 1
#define  USE_SDL 0;
#define  WAVEMAX 32767

drawPcm::drawPcm(QWidget *parent)
	: QMainWindow(parent)
{
	int size = sizeof(short);
	setFixedWidth(3000);
	setFixedHeight(1000);
	btn = new QPushButton(this);
	connect(btn,SIGNAL(clicked(bool)),this,SLOT(handleClick()));
	btn->setFixedSize(100,60);
	btn->move(0,0);
	length = 0;
	maxValue = 0;
	ptrtest = new short[102400000];
	memset(ptrtest,0,102400000);
}

drawPcm::~drawPcm()
{

}

void drawPcm::paintEvent( QPaintEvent* e )
{
	QPainter painter(this);
	painter.setPen(QPen(Qt::green));
	int index = 0;
	double value = 0;
	int length = this->length;
	if (length>0)
	{
		int winterval = ceil(length/this->width()+0.5);
		int halfHeight = this->height()>>1;
		int hinterval = 32767/halfHeight;
		int tmpValue = 0;
		int indexWave = 0;
		int yValue = 0;
		int yValue2 = 0;
		int maxValue = 0;
		int minValue = 32767;
		int stepIndex = 0;
		int sumValue = 0;
		int windowIndex = 0;
		QColor color(0, 255, 0);
		for (index = 0; index<length ; index+=winterval)
		{
			maxValue = 0;
			minValue = 32767;
			int value = 0;
			for (stepIndex = 1; stepIndex < winterval; stepIndex++) 
			{
				value++;
				yValue = ptrtest[indexWave*winterval+stepIndex];
				yValue2 = ptrtest[indexWave*winterval+stepIndex+1];
				yValue = yValue/hinterval;
				yValue2 = yValue2/hinterval;
				if (yValue<0)
				{
					yValue = halfHeight+abs(yValue);
				}
				else{
					yValue = halfHeight-yValue;
				}

				if (yValue2<0)
				{
					yValue2 = halfHeight+abs(yValue2);
				}
				else{
					yValue2 = halfHeight-yValue2;
				}
				painter.drawLine(indexWave,yValue,indexWave,yValue2);
			}
		
			/*if (maxValue>0){
			painter.fillRect(QRectF(indexWave,halfHeight-maxValue,1, maxValue), color);
			}
			else{
			painter.fillRect(QRectF(indexWave,halfHeight,1, abs(maxValue)), color);
			}

			if (minValue>0){
			painter.fillRect(QRectF(indexWave,halfHeight-minValue,1, minValue), color);
			}
			else{
			painter.fillRect(QRectF(indexWave,halfHeight,1, abs(minValue)), color);
			}*/
			indexWave++;
		}
	}
}

void drawPcm::handleClick()
{
	AVFormatContext	*pFormatCtx;
	int				i, audioStream;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	char filename[]="test.mp3";

	av_register_all();

	//支持网络流输入
	avformat_network_init();
	//初始化
	pFormatCtx = avformat_alloc_context();
	//Open
	if(avformat_open_input(&pFormatCtx,filename,NULL,NULL)!=0){
		printf("Couldn't open file.\n");
		return;
	}

	// Retrieve stream information
	if(av_find_stream_info(pFormatCtx)<0){
		printf("Couldn't find stream information.\n");
		return;
	}
	// Dump valid information onto standard error
	av_dump_format(pFormatCtx, 0, filename, false);

	mduration = pFormatCtx->duration/1000000; 
	// Find the first audio stream
	audioStream=-1;
	int dAudioTimeBase;
	for(i=0; i < pFormatCtx->nb_streams; i++)
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){

			audioStream=i;
			dAudioTimeBase = av_q2d(pFormatCtx->streams[i]->time_base) * 1000;
			break;
		}

		if(audioStream==-1){
			printf("Didn't find a audio stream.\n");
			return;
		}

		// Get a pointer to the codec context for the audio stream
		pCodecCtx=pFormatCtx->streams[audioStream]->codec;

		// Find the decoder for the audio stream
		pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
		if(pCodec==NULL){
			printf("Codec not found.\n");
			return;
		}

		// Open codec
		if(avcodec_open(pCodecCtx, pCodec)<0){
			printf("Could not open codec.\n");
			return;
		}

		//把结构体改为指针
		AVPacket *packet=(AVPacket *)malloc(sizeof(AVPacket));
		av_init_packet(packet);

		AVFrame	*pFrame;
		pFrame=avcodec_alloc_frame();

		printf("Bitrate: %3d\n", pFormatCtx->bit_rate);
		printf("Codec Name: %s\n", pCodecCtx->codec->long_name);
		printf("Channels:  %d \n", pCodecCtx->channels);
		printf("Sample per Second  %d \n", pCodecCtx->sample_rate);
		pCodecCtx->channels = 1;
		uint32_t ret,len = 0;
		int got_picture;
		int index = 0;
		int totalSize = 0;
		while(av_read_frame(pFormatCtx, packet)>=0)
		{
			if(packet->stream_index==audioStream)
			{
				ret = avcodec_decode_audio4( pCodecCtx, pFrame,&got_picture, packet);
				if ( ret < 0 ) {
					printf("Error in decoding audio frame.\n");
					exit(0);
				}
				if ( got_picture > 0 ){
					uint8_t *ptr =pFrame->data[0];		
					int sumValue = 0;
					short val;
					int testValue = 0;
					for (int i = 0; i < pFrame->linesize[0]; i += 1024)
					{
						val = (short)(
							((unsigned char)ptr[i]) << 8 | 
							((unsigned char)ptr[i+1])
							);
						ptrtest[length] = val;
						length++;
					}
				}
			}
			av_free_packet(packet);
		}
		// Close the codec
		avcodec_close(pCodecCtx);
		// Close the video file
		av_close_input_file(pFormatCtx);
		update();
		return ;
}

void drawPcm::onFinished( int code, QProcess::ExitStatus status )
{
	update();
}
