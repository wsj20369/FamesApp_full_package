#include<stdio.h>
#include<graphics.h>

typedef struct{
/*BMP 关键结构 18个字节*/
	unsigned char identifier[2];                     /* 标识: "BM" */
	unsigned long file_size;                /* 文件大小,以字节计 */
	unsigned long reserved;                 /* 保留 */
	unsigned long bitmap_data_offset;       /* BMP数据相对文件头的偏移 */
	unsigned long bitmap_header_size;       /* BMP信息头大小 */
}BmpCoreInfo;

typedef struct{
/*BMP 信息头 40个字节*/
	unsigned long width;                    /* BMP 宽度，以象素计  */
	unsigned long height;                   /* BMP 高度，以象素计  */
	unsigned int  planes;                   /* 帧数(位面数) */
	unsigned int  bits_per_pixel;           /* 象素位宽，如: 8, 16, 24, 32, 1, 4 */
	unsigned long compression;              /* 压缩规格 */
	unsigned long bitmap_data_size;         /* BMP 数据大小，this number must be rounded to the next 4 byte boundary */
	unsigned long hresolution;              /* 水平分辨率? */
	unsigned long vresolution;              /* 垂直分辨率? */
	unsigned long colors;                   /* 颜色数 */
	unsigned long important_colors;         /* 重要颜色数 */
}BmpInfoHeader;

char FontBuffer[30*1024];
char FontModule[3 *1024];		/* 一个字模的缓冲 */
int  FontWidth;			/* 字模宽度 */
int  FontHeight;		/* 字模高度 */
int  FontNum;			/* 字模个数 */

int  rowwidth;

int GetImage(char *filename)
{
BmpCoreInfo     bmpcoreinfo;
BmpInfoHeader   bmpinfoheader;
FILE *          fp;
int             row;

fp=fopen(filename,"rb");
if(!fp)return 1;
fread((void *)&bmpcoreinfo, sizeof(BmpCoreInfo), 1, fp);
if((bmpcoreinfo.identifier[0]!='B')||(bmpcoreinfo.identifier[1]!='M'))return 2;
fread((void *)&bmpinfoheader, sizeof(BmpInfoHeader), 1, fp);
if((bmpinfoheader.bits_per_pixel!=1))return 4;
fseek(fp, bmpcoreinfo.bitmap_data_offset, SEEK_SET);
rowwidth=(bmpinfoheader.bitmap_data_size / bmpinfoheader.height);
for(row=(bmpinfoheader.height-1); row>=0; row--){
	fread((void *)&FontBuffer[row*rowwidth], rowwidth,1, fp);
	if(feof(fp)){ fclose(fp); return 3; }
}
fclose(fp);
return 0;
}

die(char *s)
{
printf("\n%s\n",s);
exit(0);
}

main(int argc, char **argv)
{
char bmpfile[32];
char fontfile[32];
FILE *font;
int  tmp1,tmp2,tmp3, tmp4, tmp5;
char c;
int GD=VGA,GM=VGAHI,i,j,x,y;

printf("Pixel Font Generator\n");
printf("Input bmp File: ");
scanf("%s", bmpfile);
printf("Input font File: ");
scanf("%s", fontfile);
printf("Input Font Height: ");
scanf("%d", &FontHeight);
printf("Input Font Width: ");
scanf("%d", &FontWidth);
printf("Input Font Number: ");
scanf("%d", &FontNum);
if(GetImage(bmpfile)){
	printf("Bmp file error!\n");
	return;
}
font=fopen(fontfile, "wb");
if(font==NULL){
	printf("Font file create error!\n");
	return;
}

#define GetBIT(buf, row, rowwidth, pos)		((buf[(row)*(rowwidth)+(pos)/8])&(0x80>>((pos)%8)))
#define SetBIT(buf, row, rowwidth, pos)		((buf[(row)*(rowwidth)+(pos)/8])|=(0x80>>((pos)%8)))
#define ClrBIT(buf, row, rowwidth, pos)		((buf[(row)*(rowwidth)+(pos)/8])&=(~(0x80>>((pos)%8))))


for(tmp1=0; tmp1<FontNum; tmp1++)
{
	for(tmp4=0;tmp4<FontHeight;tmp4++)
		for(tmp5=0;tmp5<FontWidth;tmp5++){
			if(GetBIT(FontBuffer, tmp4, rowwidth, ((tmp1*FontWidth)+tmp5)))
				SetBIT(FontModule, tmp4, ((FontWidth+7)/8), tmp5);
			else
				ClrBIT(FontModule, tmp4, ((FontWidth+7)/8), tmp5);
		}
	fwrite(FontModule, ((FontWidth+7)/8)*FontHeight, 1, font);
}
fclose(font);
printf("Make Font OK\n");
font=fopen(fontfile, "rb");
if(font==NULL){
	printf("Font file open error!\n");
	return;
}
tmp1=FontHeight*((FontWidth+7)/8);
tmp2=tmp3=10;
x=tmp3;
y=tmp2-FontWidth-4;
initgraph(&GD,&GM,"c:\\tc");
for(c=0;c<FontNum;c++){
fseek(font, c*tmp1, SEEK_SET);
fread(FontModule, tmp1,1, font);
y+=FontWidth+4;
if(y+FontWidth>=639){ 
	x+=(FontHeight+4);
	y =tmp2;
}
for(i=x; i<x+FontHeight;i++)
   for(j=y; j<y+FontWidth; j++){
	if(GetBIT(FontModule, (i-x), ((FontWidth+7)/8), (j-y)))putpixel(j,i,BLUE);
	else putpixel(j,i,YELLOW);
}
}
getch();
closegraph();
}


















