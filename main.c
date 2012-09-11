////////////////////////////////////////////////////////////////////////////////
//
// Petit programme de test de BlueZ
//
////////////////////////////////////////////////////////////////////////////////

#include "test.h"

/* Adresse du socket vers l'adapteur */
int dd;

void f_exit(void)
{
    hci_close_dev(dd);
}

int main (int argc, char *argv[])
{
    if (atexit(f_exit))
    {
        perror("Cannot set exit function.\n");
        exit(EXIT_FAILURE);
    }
    
    int i,
        /* Accès à l'adateur local*/
        dev_id,
        /* Nombre de peripheriques à portées obtenus par le scan */
        num_rsp,
        /* adresse du socket de communication crée pour le dialogue par RFCOMM*/
        rfcoms,
        /* Statut de la connection RFCOMM au péripherique distant */
        status;
        
        /* Tableau des noms péripheriques découverts lors de la recherche */
    btdname name[MAX_RSP];
        /* Tableay des adresses des péripheriques découverts lors de la recherche */
    btdaddr addr[MAX_RSP];
    
    printf("Please wait 10 seconds or the scan of %d devices to end...\n", 
            MAX_RSP);
    
    /* On récupère l'accès à l'adapteur bluetooth */
    dev_id = hci_get_route(NULL);
    /* On ouvre un socked de communication vers l'adaptateur bluetooth */
    dd = hci_open_dev(dev_id);
    
    if (dev_id < 0 || dd < 0) {
        perror("opening socket");
        exit(EXIT_FAILURE);
    }
    
    /* resultats du scan */
    inquiry_info *ii = NULL;
    ii = (inquiry_info*)malloc(MAX_RSP * sizeof(inquiry_info));
    
    /* Decouverte des peripheriques à portée */
    num_rsp = hci_inquiry(dev_id, LEN, MAX_RSP, NULL, &ii, IREQ_CACHE_FLUSH);
    printf("done.\n");
    
    for (i = 0; i < num_rsp; i++)
    {
        if(hci_read_remote_name(dd, &(ii+i)->bdaddr, 
                sizeof(name[i]), name[i], 0) < 0)
            strcpy(name[i], "[unknown]");
        
        if(!ba2str(&(ii[i].bdaddr), addr[i]))
        {
            strcpy(addr[i], "UN:KN:OW:NE:RR:OR");
        }
        
        printf("%d)    %s    %s    (class 0x%02X%02X%02X))\n",
                i+1, addr[i], name[i], ii[i].dev_class[2],
                ii[i].dev_class[1], ii[i].dev_class[0]);
    }
    free(ii);
    
    if (num_rsp == 0)
    {
        printf("No bluetooth devices found.\n");
        exit(EXIT_SUCCESS);
    }
    
    int input = -1;
    while (input == -1 || input > num_rsp)
    {
        printf("Select the bluetooth device (0 to quit): \n");
        scanf("%d", &input);
        if (input == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else if (input == -1 || input > num_rsp+1)
        {
            printf("Invalid input, please try again.\n");
        }
        
        /* on vide le buffer de l'entrée standard */
        int c = 0;
        while (c != '\n' && c != EOF)
        {
            c = getchar();
        }
    }
    
    struct sockaddr_rc destaddr = {0};
    
    printf("Connecting to remote device %s\n", addr[input-1]);
    
    rfcoms = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    destaddr.rc_family = AF_BLUETOOTH;
    destaddr.rc_channel = (uint8_t) 1;
    str2ba(addr[input-1], &destaddr.rc_bdaddr);
    
    printf("...\n");
    status = connect(rfcoms, (struct sockaddr *) &destaddr, sizeof(destaddr));
    printf("Status : %d\n", status);
    
    if (status == 0)
    {
        /* On est connecté ! */
        printf("Connected !\n");
		printf("Sending message...\n");
		write(rfcoms, "Hello...", 2);
		sleep(2);
		write(rfcoms, "world !", 2);
		printf("Done sending messages through rfcomm.\n");
    }
    else if (status == -1)
    {
        printf("Connection failed.\n");
    }
    close(rfcoms);
    
    return (EXIT_SUCCESS);
}
