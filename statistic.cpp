#include <map>
#include <fstream>
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;


struct item{
	int age;
	char workclass[32];
	int fnlwgt;
	char education[32];
	int education_num;
	char marital_status[32];
	char occupation[32];
	char relationship[32];
	char race[32];
	char sex[32];
	int capital_gain;
	int capital_loss;
	int hours_per_week;
	char native_country[32];
	char result[32];
	
};

#define Get_map_count(map,Key) do{     \
	if(map.count(Key)>0)           \
		map[Key]++;            \
	else                           \
		map[Key] = 1;	       \
}while(0)          


#define MAX_LEN 1024

static unsigned int discard_lines=0;
static unsigned int current_line=0;

bool pre_process(const char* filename,const char* newfilename) //replace ',' by ' ' 
{
    FILE *f=NULL,*fnew=NULL;
    char buf[MAX_LEN];
    unsigned int read=0,len;

    f = fopen(filename,"r");
    fnew = fopen(newfilename,"w");
    if(!f){
        return false;
    }
    if(!fnew){
        fclose(f);
        return false;
    }

    while(!feof(f)){
       if( fgets(buf,MAX_LEN,f)!=NULL)
       {
            char *p = buf;
            bool discard_flag = false;
			++current_line;  
			  	

	    while(*p!='\n')
            {
                    
                    if(*p=='?')
                    {
                            discard_lines++;
                            discard_flag=true;	
                            break;
                    }else if(*p==','){
			*p =' ';
		    }
                    p++;
            }
            if(!discard_flag)
            {
                    fputs(buf,fnew);
            }
       }
    }
    fclose(f);
    fclose(fnew);
}

void process_string_attribute(const char*filename) //统计字符串属性
{
	struct item entry;
	FILE * f=NULL;
	map<string,int> stat[8];
	ofstream out("string_statistic.data");
	f = fopen(filename,"r");
	while(!feof(f))
	{
		fscanf(f,"%d  %s  %d  %s  %d  %s  %s  %s  %s  %s  %d  %d  %d  %s  %*s",\ 
		&entry.age, entry.workclass, &entry.fnlwgt, entry.education, &entry.education_num,\
		entry.marital_status, entry.occupation, entry.relationship, entry.race, entry.sex,\
		&entry.capital_gain, &entry.capital_loss, &entry.hours_per_week, entry.native_country);	   		
		
		string str = entry.workclass;
		Get_map_count(stat[0],str);
		str = entry.education;
		Get_map_count(stat[1],str);
		str = entry.marital_status;
		Get_map_count(stat[2],str);
		str = entry.occupation;
		Get_map_count(stat[3],str);
		str = entry.relationship;
		Get_map_count(stat[4],str);
		str = entry.race;
		Get_map_count(stat[5],str);
		str = entry.sex;
		Get_map_count(stat[6],str);
		str = entry.native_country;
		Get_map_count(stat[7],str);
		
	}
	for(int i=0;i<8;i++)
	{
		for(map<string,int>::iterator p = stat[i].begin();p!=stat[i].end();p++)
		{
			out<<"属性："<<p->first<<" 出现次数："<<p->second<<endl;
		}
		out<<"................."<<endl;
	}

}


struct digit_attribute{
	int age;
	int education_num;
	int capital_gain;
	int capital_loss;
	int hours_per_week;
};

class classcomp {
public:
	bool operator() (const struct digit_attribute& lhs, const struct digit_attribute& rhs) const
  	{
		const int *a=&lhs.age,*b=&rhs.age;
		int index=0;
		while(index<sizeof(struct digit_attribute)/sizeof(int) && *a==*b)
		{
			index++;
			a++;
			b++;
		}
		if(index==5)
			return false;
		return *a<*b;
	}
	
};

void process_digit_attribute(const char*filename) //统计数字属性
{
	FILE *f=NULL,*out=NULL;
	int count =0;
	struct digit_attribute d;
	map<struct digit_attribute,int,classcomp> digit_map;

	out = fopen("digit_statistic.data","w");
	fprintf(out,"age  education-num  capital-gain  capital-loss  hours-per-week  count\n");
	f = fopen(filename,"r");
	while(!feof(f) && count++<30000)
	{
		fscanf(f,"%d  %*s  %*d  %*s  %d  %*s  %*s  %*s  %*s  %*s  %d  %d  %d  %*s  %*s", \
		&d.age, &d.education_num, &d.capital_gain, &d.capital_loss, &d.hours_per_week);
		//fprintf(out,"%d %d %d %d %d\n",d.age,  d.education_num, d.capital_gain, d.capital_loss, d.hours_per_week);
		Get_map_count(digit_map,d);
	}
	for(map<struct digit_attribute,int,classcomp>::iterator p = digit_map.begin();p!=digit_map.end();p++)
	{
		const struct digit_attribute* t = &(p->first);
		fprintf(out,"%-5d%-15d%-14d%-14d%-16d%-14d\n",t->age,t->education_num, t->capital_gain, \
			t->capital_loss, t->hours_per_week, p->second);	
	}
}


int main()
{
	pre_process("adult.data","fnew.data");
	printf("total lines:%d\ndiscard lines: %d\n",current_line,discard_lines);
	
	process_string_attribute("fnew.data"); //统计字符串属性
	process_digit_attribute("fnew.data"); //统计数字属性

	return 0;
}
