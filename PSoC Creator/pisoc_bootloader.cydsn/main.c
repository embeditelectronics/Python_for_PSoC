/* ========================================
 *
 * Embedit Electronics
 *
 * PiSoC Rev E bootloader
 * 
 * MIT License
 *
 * ========================================
*/
#include <project.h>

int main()
{
    //CyGlobalIntEnable; /* Enable global interrupts. */
    //CyDelay(10);
    //checks if bootloader pin is low, if it is the bootloader is started
    //otherwise go to the user application (bootloadable)
    if((BOOT_MODE_Read() == 0) || (Bootloader_GET_RUN_TYPE == Bootloader_START_BTLDR))
	{	
		Bootloader_SET_RUN_TYPE(Bootloader_START_BTLDR);
	}
    
    /*if(BOOT_MODE_Read() == 0)
    {
        //Start Boot loader
        Bootloader_Start();
        //CyBtldr_Start();
    }*/
    
    CyBtldr_Start();
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
