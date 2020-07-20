#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define clear() printf("\033[H\033[J");

int src, dest, count;
char buf[512];
int status = 0;

void call_tasi(char fileName[], char dest[]);

void start_shell()
{
    clear();
    printf("\n--------------------------------------------------------------\n");
    printf("|                  Turkce Linux Komut Terminali                 |\n");
    printf("|                      Gelistiren gyurtalan                     |\n");
    printf("--------------------------------------------------------------\n");
}
void prompt()
{
    //current directory
    char current[1000];
    getcwd(current, sizeof(current));
    printf("%s", current);
    //prompt
    printf(": komut> ");
}
int get_input(char *input)
{
    scanf("%[^\n]", input);
    ;
    getchar(); //the next character
    if (strlen(input) == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
void call_goster()
{
    // goster command
    printf("Mevcut klasor adresi:\n");
    char current[1000];
    getcwd(current, sizeof(current));
    printf("%s\n", current);
}
void call_gec(char **word)
{
    // gec command
    if (chdir(word[1]) == -1)
    {
        printf("Klasor bulunamadi\n");
    }
    else
    {
        printf("Klasor Degistirildi\n");
    }
}
void call_kolustur(char **word, int input_length)
{
    // kolustur command
    for (int i = 1; i < input_length; i++)
    {
        if (mkdir(word[i], 0777) == -1)
        {
            printf("'%s' klasoru zaten mevcut!!!\n", word[i]);
        }
        else
        {
            printf("'%s' klasoru basari ile olusturuldu \n", word[i]);
        }
    }
}
void call_sil(char **word, int input_length)
{
    // sil command
    for (int i = 1; i < input_length; i++)
    {
        if (rmdir(word[i]) == -1)
        {
            printf("'%s' silinecek klasor bulunamadi!!!\n", word[i]);
        }
        else
        {
            printf("Klasor '%s' basari ile silindi\n", word[i]);
        }
    }
}

void call_tasi(char srcFile[], char destFile[])
{
    // tasi command
    src = open(srcFile, O_RDONLY);

    if (src == -1)
    {
        printf("Kaynak dosya mevcut degil!!!\n");
        exit(0);
    }
    dest = open(destFile, O_WRONLY);
    //Create if file not present already
    if (dest == -1)
        dest = creat(destFile, 0766);

    //Read from the source file and write to the destination file
    while ((count = read(src, buf, 512)) > 0)
    {
        write(dest, buf, count);
    }
    close(src);
    close(dest);

    count = unlink(srcFile);
}

void call_dolustur(char srcFile[], char descrp[])
{
    // dolustur command

    dest = creat(srcFile, 0766);
    write(dest, (u_int8_t *)descrp, strlen(descrp));
    close(dest);
}

void call_hafiza_takip()
{
    // hafiza_takip command
    int n = 20;
    int entrySize = 10000000;
    int *buffer[n];
    long vmrss, vmsize;

    for (int i = 0; i < n; i++)
    {
        buffer[i] = malloc(entrySize * sizeof(int));

        if (!buffer[i])
        {
            printf("Couldn't allocate memory!\n");
            exit(1);
        }

        for (int j = 0; j < entrySize; j++)
        {
            buffer[i][j] = 0;
        }

        get_memory_usage_kb(&vmrss, &vmsize);
        printf("%2d: |Mevcut hafiza kullanimi: VmRSS = %6ld KB, VmSize = %6ld KB\n",
               i, vmrss, vmsize);
    }
}

int get_memory_usage_kb(long *vmrss_kb, long *vmsize_kb)
{
    /* Get the the current process' status file from the proc filesystem */
    FILE *procfile = fopen("/proc/self/status", "r");

    long to_read = 8192;
    char buffer[to_read];
    int read = fread(buffer, sizeof(char), to_read, procfile);
    fclose(procfile);

    short found_vmrss = 0;
    short found_vmsize = 0;
    char *search_result;

    /* Look through proc status contents line by line */
    char delims[] = "\n";
    char *line = strtok(buffer, delims);

    while (line != NULL && (found_vmrss == 0 || found_vmsize == 0))
    {
        search_result = strstr(line, "VmRSS:");
        if (search_result != NULL)
        {
            sscanf(line, "%*s %ld", vmrss_kb);
            found_vmrss = 1;
        }

        search_result = strstr(line, "VmSize:");
        if (search_result != NULL)
        {
            sscanf(line, "%*s %ld", vmsize_kb);
            found_vmsize = 1;
        }

        line = strtok(NULL, delims);
    }

    return (found_vmrss == 1 && found_vmsize == 1) ? 0 : 1;
}

void call_cik()
{
    // cik command
    printf("Cik Shell\n");
    status = 1;
    exit(0);
    exit(1);
}
void call_yardim()
{

    printf("\n");
    printf("****************************************************\n");
    printf("****************************************************\n");
    printf("\n");
    printf("  Yardim Menusu:\n");
    printf("\n");
    printf("  gyurtalan tarafindan gelistirilmistir.\n");
    printf("  Gecerli komut ve parametreler asagida listelenmistir:\n");
    printf("\n");
    printf("****************************************************\n");
    printf("****************************************************\n");
    printf("goster                       // mevcut dizini gosterir \n");
    printf("\n");
    printf("gec .. | gec /home/user      // dizinler arasi gecisi saglar \n");
    printf("\n");
    printf("kolustur  /home/user/deneme  // yeni klasor olusturur\n");
    printf("\n");
    printf("sil  /home/user/deneme       // hedef klasoru siler\n");
    printf("\n");
    printf("tasi  /home/source/test.txt /home/destination/test.txt     // kaynak dosyayi hedefe tasir \n");
    printf("\n");
    printf("dolustur  /home/source/test.txt merhaba dunya     // hedef dosyayi icerik ile birlikte yaratir \n");
    printf("\n");
    printf("hafiza                       // mevcut hafiza durumunu gosterir \n");
    printf("\n");
    printf("cik                          // mevcut terminali sonlandirir\n");
    printf("\n");
    printf("****************************************************\n");
    printf("\n");
}

int main()
{
    //declarations
    int x = 0, input_length = 0;
    char input[1000];
    char str_inp[1000];
    pid_t pid;
    //starting the shell
    start_shell();
    //loop
    while (1)
    {
        //the current directory and prompt
        prompt();
        //get input from the user
        x = get_input(input);
        if (x == 0)
        {
            continue;
        }
        else
        {

            char *word[100];  //words in the current input
            input_length = 0; //number of words in current input
            status = 0;       // for keep wait parent process
            strcpy(str_inp, input);
            char *ptr = strtok(input, " ");
            while (ptr != NULL)
            {
                word[input_length] = ptr;
                ptr = strtok(NULL, " ");
                input_length++;
            }

            if ((pid = fork()) == 0)
            {
                //Turkish shell commands
                if (strcmp(word[0], "goster") == 0) //pwd shell
                {
                    if (input_length == 1)
                    {
                        call_goster();
                    }
                }
                else if (strcmp(word[0], "gec") == 0)
                { // cd shell
                    if (input_length == 1)
                    {
                        printf("Lutfen gec 'klasor yolu' komutu giridiz  \n");
                    }
                    else if (input_length > 2)
                    {
                        printf("Cok fazla parametre girdiniz. Lutfen gec 'klasor yolu' komutu giridiz  \n");
                    }
                    else
                    {
                        call_gec(word);
                    }
                }
                else if (strcmp(word[0], "kolustur") == 0) // mkdir klasor olustur
                {
                    if (input_length == 1)
                    {
                        printf("Lutfen 'klasor_yolunu' giriniz\n");
                    }
                    else
                    {
                        call_kolustur(word, input_length);
                    }
                }
                else if (strcmp(word[0], "sil") == 0) // rmdir
                {
                    if (input_length == 1)
                    {
                        printf("Lutfen 'klasor_yolunu' giriniz\n");
                    }
                    else
                    {
                        call_sil(word, input_length);
                    }
                }

                else if (strcmp(word[0], "tasi") == 0) //mv shell
                {
                    if (input_length == 3)
                    {
                        call_tasi(word[1], word[2]);
                    }
                    else
                    {
                        printf("tasima islemi icin lutfen dosya adini ve hedef klasor giriniz\n");
                        printf("Ornek: tasi deneme.txt /home/user/downloads \n");
                    }
                }

                else if (strcmp(word[0], "dolustur") == 0) //cat shell
                {
                    if (input_length >= 3)
                    {
                        call_dolustur(word[1], word[2]);
                    }
                    else
                    {
                        printf("yazma islemi icin lutfen dosya adini ve aciklama giriniz\n");
                        printf("Ornek: dolustur /home/user/deneme.txt merhaba dunya \n");
                    }
                }

                else if (strcmp(word[0], "hafiza") == 0) //custom shell
                {
                    if (input_length == 1)
                    {
                        call_hafiza_takip();
                    }
                    else
                    {
                        printf("hafiza takibi icin 'hafiza' komutunu kullaniniz!!!\n");
                    }
                }

                else if (strcmp(word[0], "cik") == 0)
                {
                    if (input_length > 1)
                    {
                        printf("lutfen sadece 'cik' komutu ile cikis yapiniz\n");
                    }
                    else
                    {
                        call_cik();
                    }
                }
                else if (strcmp(word[0], "yardim") == 0)
                {
                    call_yardim();
                }
                else
                {
                    printf("Bilinmeyen komut\n");
                }
                status = 1;
                printf("child pid: %s", pid);
                kill(getpid(), SIGUSR1);
            }
            else
            {
                // Parent process
                do
                {
                    // printf("parent pid: '%s'\n", pid);
                    waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }
        }
    }
}
