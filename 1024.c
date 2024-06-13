#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
/*
1)Note that given code fully utilizes spaces that is assigned i.e array of unsigned long long int.
2)64*16 bits are fully utilized here.
3)Input is stored in array of size 64*16 bits.
4)Overflow condition is also considered.
5)Negative numbers arithmetic is also taken into consideration.
*/
typedef struct BigInt_tag
{
    unsigned long long int arr[16];
    char sign;
}BigInt;
void initialize(BigInt*,char*);
//checks whether integer entered by user is valid or not
bool isvalid(char*s)
{
    char temp[1025];
    strcpy(temp,s);
    bool ans=true;
    for(int i=0;(i<strlen(s))&&ans==true;i++)
    {
        if(s[i]>'9'||s[i]<'0')
        {
            ans=false;
        }
    }
    if(ans==true)
    {
        if(strlen(s)<309)
        {
            char arr[309-strlen(s)];
            arr[0]='\0';
            for(int i=0;i<309-strlen(s);i++)
            {
                char ch[2]={'0','\0'};
                strcat(arr,ch);
            }
            char arr1[1025];
            arr1[0]='\0';
            strcat(arr1,s);
            strcat(arr1,arr);
            s[0]='\0';
            strcpy(s,arr1);
        }
        else if(strlen(s)>309)
        {
            ans=false;
        }
        else
        {
            char maxnum[310]="179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137215";
            int i=0,cnt=0;
            while(i<310&&s[i]<=maxnum[i])
            {
                if(s[i]==maxnum[i])
                {
                    ++cnt;
                }
                ++i;
            }
            if(cnt==i&&i!=310)
            {
                ans=false;
            }
        }
        strcpy(s,temp);
    }
    return ans;
}
//converts decimal string to binary
char* toBinary(BigInt b1)
{
    char*ans=(char*)malloc(1025*sizeof(char));
    ans[0]='\0';
    int max=63;
    int i=0;
    for(;i<=15;i++)
    {
        while(max>=0)
        {
            if(((unsigned long long)1<<max)&b1.arr[i])
            {
                char ch[2]={'1','\0'};
                strcat(ans,ch);
            }
            else
            {
                char ch[2]={'0','\0'};
                strcat(ans,ch);
            }
            max--;
        }
        max=63;
    }
    return ans;
}
//convers binary string to decimal
void binarytodecimal(char*s,BigInt*b3)
{
    char term[310],ans[310];
    for(int i=0;i<310;i++)
    {
        ans[i]='0';
        term[i]='0';
    }
    ans[309]='\0';
    term[308]='1';
    term[309]='\0';
    int carry=0;
    for(int i=strlen(s)-1;i>=0;i--)
    {
        if(i!=strlen(s)-1)
        {
            carry=0;
            for(int j=308;j>=0;j--)
            {
                int s1=2*(term[j]-'0');
                term[j]=(s1+carry)%10+'0';
                carry=(s1+carry)/10;
            }
        }
        if(s[i]=='1')
        {
            carry=0;
            for(int j=308;j>=0;j--)
            {
                int s=ans[j]+term[j]-2*'0';
                ans[j]=(s+carry)%10+'0';
                carry=(s+carry)/10;
            }
        }
    }
    int i=0;
    while(i<309&&ans[i]=='0')
    {
        i++;
    }
    printf("%s",ans+i);
    if(b3&&b3->sign=='1')
    {
        char arr[1026]={'-','\0'};
        strcat(arr,ans+i);
        initialize(b3,arr);
    }
    else
    {
        char arr[1026];
        strcpy(arr,ans+i);
        initialize(b3,arr);
    }
}
//add operation
void add(BigInt b1,BigInt b2,BigInt* b3)
{
    char*op1=toBinary(b1);
    char*op2=toBinary(b2);
    char ans[1025];
    ans[1024]='\0';
    int carry=0;
    if(b1.sign==b2.sign)
    {
        for(int i=1023;i>=0;i--)
        {
            ans[i]=(op1[i]+op2[i]-2*'0'+carry)%2+'0';
            carry=(op1[i]+op2[i]-2*'0'+carry)/2;
        }
        if(carry==0)
        {
            if(b1.sign=='1')
            {
                printf("-");
                if(b3)
                {
                    b3->sign='1';
                }
            }
            else
            {
                if(b3)
                {
                    b3->sign='0';
                }
            }
            binarytodecimal(ans,b3);
            free(op1);
            free(op2);
        }
        else
        {
            printf("Overflow\n");
        }
    }
    else
    {
        carry=1;
        ans[0]='0';
        for(int i=1023;i>=0;i--)
        {
            ans[i]=(op1[i]+1-(op2[i]-'0')-'0'+carry)%2+'0';
            carry=(op1[i]+1-(op2[i]-'0')-'0'+carry)/2;
        }
        if(carry==0)
        {
            carry=1;
            if(b1.sign=='0')printf("-");
            for(int i=1023;i>=0;i--)
            {
                ans[i]=(1-(ans[i]-'0')+carry)%2+'0';
                carry=(1-ans[i]+'0'+carry)/2;
            }
            ans[0]='0';
        }
        else
        {
            if(b1.sign=='1')
            {
                printf("-");
                if(b3)
                {
                    b3->sign='1';
                }
            }
            else
            {
                if(b3)
                {
                    b3->sign='0';
                }
            }
        }
        binarytodecimal(ans,b3);
        free(op1);
        free(op2);
    }
}
//subtract operation
void subtract(BigInt b1,BigInt b2,BigInt*b3)
{
    char*arr1=toBinary(b1);
    char*arr2=toBinary(b2);
    int carry;
    char arr3[1025];
    arr3[1024]='\0';
    carry=1;
    arr3[0]='0';
    if(b1.sign==b2.sign)
    {
        if(b1.sign=='1')
        {
            char*temp=arr1;
            arr1=arr2;
            arr2=temp;
        }
        for(int i=1023;i>=0;i--)
        {
            arr3[i]=(arr1[i]+1-(arr2[i]-'0')-'0'+carry)%2+'0';
            carry=(arr1[i]+1-(arr2[i]-'0')-'0'+carry)/2;
        }
        if(carry==0)
        {
            carry=1;
            printf("-");
            if(b3)
            {
                b3->sign='1';
            }
            for(int i=1023;i>=0;i--)
            {
                arr3[i]=(1-(arr3[i]-'0')+carry)%2+'0';
                carry=(1-arr3[i]+'0'+carry)/2;
            }
            arr3[0]='0';
        }
        else
        {
            if(b3)
            {
                b3->sign='0';
            }
        }
        binarytodecimal(arr3,b3);
        free(arr1);
        free(arr2);
    }
    else
    {
        carry=0;
        for(int i=1023;i>=0;i--)
        {
            arr3[i]=(arr1[i]+arr2[i]-2*'0'+carry)%2+'0';
            carry=(arr1[i]+arr2[i]-2*'0'+carry)/2;
        }
        if(carry==0)
        {
            if(b1.sign=='1')
            {
                printf("-");
                if(b3)
                {
                    b3->sign='1';
                }
            }
            else
            {
                if(b3)
                {
                    b3->sign='0';
                }
            }
            binarytodecimal(arr3,b3);
            free(arr1);
            free(arr2);
        }
        else 
        {
            printf("Overflow\n");
        }
    }
}
//multiplication operation
void multiply(BigInt b1,BigInt b2,BigInt*b3)
{
    char*arr1=toBinary(b1);
    char*arr2=toBinary(b2);
    char arr3[2049];
    arr3[2048]='\0';
    for(int i=0;i<2048;i++)arr3[i]='0';
    for(int i=1023;i>=0;i--)     
    {
        int carry=0;
        char temp[2049];
        for(int l=0;l<2048;l++)temp[l]='0';
        for(int j=1023;j>=0;j--)
        {
            temp[2048-strlen(arr1)+j-strlen(arr2)+1+i]=((arr2[i]-'0')*(arr1[j]-'0')+carry)%2+'0';
            carry=((arr2[i]-'0')*(arr1[j]-'0')+carry)/2;
        }
        temp[2048-strlen(arr1)+i-strlen(arr2)]=carry+'0';
        carry=0;
        for(int k=2047;k>=0;k--)
        {
            char t=arr3[k];
            arr3[k]=(arr3[k]-'0'+temp[k]-'0'+carry)%2+'0';
            carry=(t+temp[k]-2*'0'+carry)/2;
        }
    }
    int i=0;
    while(arr3[i]=='0'&&i<1024)++i;
    if(i<1024)
    {
        printf("Overflow");
        exit(EXIT_FAILURE);
    }
    else
    {
        if(b1.sign!=b2.sign)
        {
            printf("-");
            if(b3)
            {
                b3->sign='1';
            }
        }
        else
        {
            if(b3)
            {
                b3->sign='0';
            }
        }
        binarytodecimal(arr3+i,b3);
        free(arr1);
        free(arr2);
    }
}
//converts input decimal string given by user into 1024 bits binary number
void convert(char*str)
{
    char str1[1025],ptr=0;
    int flag=1;
    str1[0]='\0';
    for(;flag==1;)
    {
        flag=0;
        for(int j=0;j<strlen(str);j++)
        {
            if(str[j]!='0')
            {
                flag=1;
            }
            char s=str[j];
            str[j]=((str[j]-'0')/2)+'0';
            if(j+1!=strlen(str))
            {
                str[j+1]=(str[j+1]+10*((s-'0')%2));
            }
            else
            {
                char ch[2]={(s-'0')%2+'0','\0'};
                strcat(str1,ch);
            }
        }
    }
    strrev(str1);
    if(strlen(str1)<1024)
    {
        char temp[1025];
        temp[0]='\0';
        for(int i=0;i<1024-strlen(str1);i++)
        {
            char ch[2]={'0','\0'};
            strcat(temp,ch);
        }
        strcat(temp,str1);
        strcpy(str1,temp);
    }
    str[0]='\0';
    if(strlen(str1)<=1024)
    {
        strcat(str,str1);
    }
    else 
    {
        strcat(str,str1+1);
    }
}
//Storing the number given by user in struct BigInt efficiently
void initialize(BigInt*num,char*s)
{
    if(num)
    {
        bool sc;
        if(s[0]!='-')
        {
            sc=isvalid(s);
        }
        else
        {
            sc=isvalid(s+1);
        }
        if(sc)
        {
            if(s[0]!='-')
            {
                convert(s);
                num->sign='0';
            }
            else 
            {
                convert(s+1);
                num->sign='1';
                s=s+1;
            }
            int size=64,cnt=15;
            unsigned long long mul=1;
            for(int i=0;i<16;i++)num->arr[i]=0;
            for(int i=1023;i>=0;i--)
            {
                if(size>0)
                {
                    num->arr[cnt]+=mul*(unsigned long long int)(s[i]-'0');
                    size--;
                }
                else
                {
                    --cnt;
                    size=64;
                    mul=1;
                    num->arr[cnt]+=mul*(s[i]-'0');
                    size--;
                }
                if(i!=0)
                {
                    mul=mul*2;
                }
            }
        }
        else 
        {
            printf("overflow\n");
            exit(EXIT_FAILURE);
        }
    }
}
void main()
{
    BigInt b1,b2,b3;
    char s[1026];
    printf("Enter first number : ");
    scanf("%s",s);
    initialize(&b1,s);
    printf("Enter second number : ");
    scanf("%s",s);
    initialize(&b2,s);
    add(b1,b2,NULL);//Normal print in console
    printf("\n");
    subtract(b1,b2,&b3);//result is stored in b3
    printf("\n");
    multiply(b1,b2,&b3);
}
