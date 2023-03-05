# include "stdio.h"
# include "math.h"
//下面把字符中的""需把转义符\放在前面
float ture_vale;
unsigned char vale[] = ":{\"method\":\"thing.service.property.set\",\"id\":\"1038159823\",\"params\":{\"valu\":18.99},\"version\":\"1.0.0\"}";
float getvalu(unsigned char *val)
{
	float output;
	unsigned char loop;
	char wanwei,qianwei,baiwei,shiwei,gewei;
	for(loop = 0;loop<255;loop++)
	{
		if(val[loop] == 'v')
			if(val[loop+1] == 'a')
				if(val[loop+2] =='l')
					if(val[loop+3] =='u')
						if(val[loop+4] =='\"')
						{
							if(val[loop+7] == '.')//数值大小为0.几的情况或者1.几的情况    测试数字   0.33  8.99
							{
								wanwei = 0;qianwei=0;
								baiwei=val[loop+6]%0x30;
								shiwei = val[loop+8]%0x30;gewei=val[loop+9]%0x30;
							}
							if(val[loop+8] == '.')   //测试  18.99
							{
								wanwei=0;
								qianwei=val[loop+6]%0x30;
								baiwei=val[loop+7]%0x30;
								shiwei = val[loop+9]%0x30;gewei=val[loop+10]%0x30;								
							}
							if(val[loop+9] == '.')   //测试  188.99
							{
								wanwei =val[loop+6]%0x30;
								qianwei=val[loop+7]%0x30;
								baiwei=val[loop+8]%0x30;
								shiwei = val[loop+10]%0x30;
								gewei=val[loop+11]%0x30;									
							}
						}
	}
	output = (wanwei*10000+qianwei*1000+baiwei*100+shiwei*10+gewei)/100.0;
	return output;
}
int mainextract(void)
{
	ture_vale = getvalu(vale);//放在单片机程序中，直接定义ture_vale变量，把存储字符串数组名字vale填入即可
	printf("%0.2f\n", ture_vale);
	return ture_vale;
}

