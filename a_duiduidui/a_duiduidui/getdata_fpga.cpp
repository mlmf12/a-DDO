#include<getdata_fpga.h>
#include <QDebug>
#define SPI_IOC_WR 123
int fd;
void set_tri_vol(unsigned char a)
{
    unsigned char rx_buf[8]={0};
    rx_buf[0]=a;
    ioctl(fd, SPI_IOC_WR, rx_buf);
}
void chuli(unsigned char *rx_buf,float *rx_buf_float,int start,unsigned char m,unsigned char offset)
{
    int stat=start*8 / m+offset;
    for(unsigned int i=0;i<8/m;i++)
    {
        int a=m*i;
        rx_buf_float[stat]=((float)rx_buf[a]*2/255-1)*5;
        stat++;
        if(stat>199)
            break;
    }
}
void chuli1(unsigned char *rx_buf,float *rx_buf_float,int start,unsigned char m,unsigned char offset)
{
    int stat=start*8 / m+offset;
    for(unsigned int i=0;i<8/m;i++)
    {
        int a=m*i;
        rx_buf_float[stat]=((float)rx_buf[a]*2/255-1)*5;
        stat++;
        if(stat>499)
            break;
    }
}
unsigned char fpgainit()
{
    char const *filename = "/dev/spi_fpga";
    fd = open(filename, O_RDWR);
    if(fd < 0)
    {
        return 0;
    }
    else return 1;
}
//带触发
unsigned char getdata_fpga(float *rx_buf_float,int *frequency,int m,unsigned char vol)
{
    unsigned char rx_buf[8] = {0};
    int status;
    int geshu=0;
    unsigned long val =0;
    unsigned char offset =0;
    unsigned char wait=0;
    int good=0;
    vol=int((float(vol)/5-4)*255/8+255/2);
    if(m==0)
      {
        while(1)
        {
            val = strtoul("55", NULL, 0);
            status = ioctl(fd, SPI_IOC_WR, rx_buf);

            unsigned char stop=0;
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0 ;
            }
            for(int b=0;b<8;b++)
            {
                //100bijiaohao
                if(wait<120)
                {
                    if((abs(rx_buf[b]-vol)<2)&&(b>0))
                    {
                        if(rx_buf[b]>rx_buf[b-1])
                        {   good=b;
                            for(int los=b;los<8;los=los+160)
                            {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                            }
                            stop++;
                            break;
                        }
                    }
                }
                else
                {
                    stop++;
                    break;
                }
            }
            wait++;
            if(stop)
                break;
        }
        for(int j = 0;j < 20*200;j++)
        {
            val = strtoul("55", NULL, 0);
            rx_buf[0] = 0;
            status = ioctl(fd, SPI_IOC_WR, rx_buf);
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0;
            }
            //de dao frequency
            int p =rx_buf[0]+rx_buf[1];
            int n =rx_buf[2]+rx_buf[3];
            int z=rx_buf[4]+rx_buf[5]+rx_buf[6]+rx_buf[7];
            if(p==0&&n==0&&z!=0)
            {
                *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
            }
            else
            {
                    int jj=j%20;
                    if(jj==0)
                    {
                    rx_buf_float[offset]=((float)rx_buf[good]*2/255-1)*5;
                    offset++;

                    }
            }


            if(offset>199)
                    break;


        }
    }

    if(m==1)
      {
        while(1)
        {
            val = strtoul("55", NULL, 0);
            status = ioctl(fd, SPI_IOC_WR, rx_buf);

            unsigned char stop=0;
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0 ;
            }
            for(int b=0;b<8;b++)
            {
                //100bijiaohao
                if(wait<120)
                {
                    if((abs(rx_buf[b]-vol)<2)&&(b>0))
                    {
                        if(rx_buf[b]>rx_buf[b-1])
                        {   good=b;
                            for(int los=b;los<8;los=los+160)
                            {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                            }
                            stop++;
                            break;
                        }
                    }
                }
                else
                {
                    stop++;
                    break;
                }
            }
            wait++;
            if(stop)
                break;
        }
        for(int j = 0;j < 2000;j++)
        {
            val = strtoul("55", NULL, 0);
            rx_buf[0] = 0;
            status = ioctl(fd, SPI_IOC_WR, rx_buf);
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0;
            }
            //de dao frequency
            int p =rx_buf[0]+rx_buf[1];
            int n =rx_buf[2]+rx_buf[3];
            int z=rx_buf[4]+rx_buf[5]+rx_buf[6]+rx_buf[7];
            if(p==0&&n==0&&z!=0)
            {
                *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
            }
            else
            {
                    int jj=j%10;
                    if(jj==0)
                    {
                    rx_buf_float[offset]=((float)rx_buf[good]*2/255-1)*5;
                    offset++;

                    }
            }


            if(offset>199)
                    break;


        }
    }


if(m==3||m==6)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<120)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los++)
                        {
                            rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                            offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 25;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli(rx_buf,rx_buf_float,geshu,1,offset);
                geshu++;
        }
        if((geshu*8+offset)>199)
                break;


    }
}
if(m==4||m==7)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<120)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los=los+2)
                        {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 51;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli(rx_buf,rx_buf_float,geshu,2,offset);
                geshu++;
        }
        if((geshu*4+offset)>199)
                break;


    }
}

if(m==5||m==2)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<120)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los=los+8)
                        {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 200;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli(rx_buf,rx_buf_float,geshu,8,offset);
                geshu++;
        }
        if((geshu*1+offset)>199)
                break;


    }
}

if(m==8)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<150)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los=los+8)
                        {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 500;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli1(rx_buf,rx_buf_float,geshu,8,offset);
                geshu++;
        }
        if((geshu*1+offset)>499)
                break;
    }
}
return geshu;
}

unsigned char getdata_fpga_single(float *rx_buf_float,int *frequency,int m,unsigned char vol)
{
    unsigned char rx_buf[8] = {0};
    int status;
    int geshu=0;
    unsigned long val =0;
    unsigned char offset =0;
    int wait=0;
    int good=0;
    vol=int((float(vol)/5-4)*255/8+255/2);
    if(m==0)
      {
        while(1)
        {
            val = strtoul("55", NULL, 0);
            status = ioctl(fd, SPI_IOC_WR, rx_buf);

            unsigned char stop=0;
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0 ;
            }
            for(int b=0;b<8;b++)
            {
                //100bijiaohao
                if(wait<500)
                {
                    if((abs(rx_buf[b]-vol)<2)&&(b>0))
                    {
                        if(rx_buf[b]>rx_buf[b-1])
                        {   good=b;
                            for(int los=b;los<8;los=los+160)
                            {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                            }
                            stop++;
                            break;
                        }
                    }
                }
                else
                {
                    stop++;
                    break;
                }
            }
            wait++;
            if(stop)
                break;
        }
        for(int j = 0;j < 20*200;j++)
        {
            val = strtoul("55", NULL, 0);
            rx_buf[0] = 0;
            status = ioctl(fd, SPI_IOC_WR, rx_buf);
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0;
            }
            //de dao frequency
            int p =rx_buf[0]+rx_buf[1];
            int n =rx_buf[2]+rx_buf[3];
            int z=rx_buf[4]+rx_buf[5]+rx_buf[6]+rx_buf[7];
            if(p==0&&n==0&&z!=0)
            {
                *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
            }
            else
            {
                    int jj=j%20;
                    if(jj==0)
                    {
                    rx_buf_float[offset]=((float)rx_buf[good]*2/255-1)*5;
                    offset++;

                    }
            }


            if(offset>199)
                    break;


        }
    }

    if(m==1)
      {
        while(1)
        {
            val = strtoul("55", NULL, 0);
            status = ioctl(fd, SPI_IOC_WR, rx_buf);

            unsigned char stop=0;
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0 ;
            }
            for(int b=0;b<8;b++)
            {
                //100bijiaohao
                if(wait<500)
                {
                    if((abs(rx_buf[b]-vol)<2)&&(b>0))
                    {
                        if(rx_buf[b]>rx_buf[b-1])
                        {   good=b;
                            for(int los=b;los<8;los=los+160)
                            {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                            }
                            stop++;
                            break;
                        }
                    }
                }
                else
                {
                    stop++;
                    break;
                }
            }
            wait++;
            if(stop)
                break;
        }
        for(int j = 0;j < 2000;j++)
        {
            val = strtoul("55", NULL, 0);
            rx_buf[0] = 0;
            status = ioctl(fd, SPI_IOC_WR, rx_buf);
            if (status < 0)
            {
                    //ui->textBrowser->setText("spi ioctl error!\n");
                    return 0;
            }
            //de dao frequency
            int p =rx_buf[0]+rx_buf[1];
            int n =rx_buf[2]+rx_buf[3];
            int z=rx_buf[4]+rx_buf[5]+rx_buf[6]+rx_buf[7];
            if(p==0&&n==0&&z!=0)
            {
                *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
            }
            else
            {
                    int jj=j%10;
                    if(jj==0)
                    {
                    rx_buf_float[offset]=((float)rx_buf[good]*2/255-1)*5;
                    offset++;

                    }
            }


            if(offset>199)
                    break;


        }
    }


if(m==3||m==6)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<500)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los++)
                        {
                            rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                            offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 25;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli(rx_buf,rx_buf_float,geshu,1,offset);
                geshu++;
        }
        if((geshu*8+offset)>199)
                break;


    }
}
if(m==4||m==7)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<500)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los=los+2)
                        {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 51;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli(rx_buf,rx_buf_float,geshu,2,offset);
                geshu++;
        }
        if((geshu*4+offset)>199)
                break;


    }
}

if(m==5||m==2)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<500)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los=los+8)
                        {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 200;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli(rx_buf,rx_buf_float,geshu,8,offset);
                geshu++;
        }
        if((geshu*1+offset)>199)
                break;


    }
}

if(m==8)
  {
    while(1)
    {
        val = strtoul("55", NULL, 0);
        status = ioctl(fd, SPI_IOC_WR, rx_buf);

        unsigned char stop=0;
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0 ;
        }
        for(int b=0;b<8;b++)
        {
            //100bijiaohao
            if(wait<500)
            {
                if((abs(rx_buf[b]-vol)<2)&&(b>0))
                {
                    if(rx_buf[b]>rx_buf[b-1])
                    {
                        for(int los=b;los<8;los=los+8)
                        {
                                rx_buf_float[offset]= ((float)rx_buf[los]*2/255-1)*5;
                                offset++;
                        }
                        stop++;
                        break;
                    }
                }
            }
            else
            {
                stop++;
                break;
            }
        }
        wait++;
        if(stop)
            break;
    }
    for(int j = 0;j < 500;j++)
    {
        val = strtoul("55", NULL, 0);
        rx_buf[0] = 0;
        status = ioctl(fd, SPI_IOC_WR, rx_buf);
        if (status < 0)
        {
                //ui->textBrowser->setText("spi ioctl error!\n");
                return 0;
        }
        //de dao frequency
        int p =rx_buf[0]+rx_buf[1];
        int n =rx_buf[2]+rx_buf[3];
        if(p==0&&n==0)
        {
            *frequency=rx_buf[5]*256*256+rx_buf[6]*256+rx_buf[7];
        }
        else
        {
                chuli1(rx_buf,rx_buf_float,geshu,8,offset);
                geshu++;
        }
        if((geshu*1+offset)>499)
                break;
    }
}
return geshu;
}
