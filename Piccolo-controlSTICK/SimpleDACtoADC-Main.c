#include "PeripheralHeaderIncludes.h"
#include "DSP2802x_EPwm_defines.h"

// FUNCTION PROTOTYPES
void DeviceInit(void);
void InitFlash();
void MemCopy(Uint16 *SourceAddr, Uint16* SourceEndAddr, Uint16* DestAddr);

// VARIABLE DECLARATIONS - GENERAL
// Used for running BackGround in flash and the ISR in RAM
extern Uint16 RamfuncsLoadStart, RamfuncsLoadEnd, RamfuncsRunStart;
Uint16 duty_cycle;

int adc_loop = 0;
int adc_result = 0;
int adc_result_last = 0;
long i;
int n;
int matched = 0;
char adc_record[1000];
char passcode[] = "oooo_o_oOoo_oOoo_OOO";			// passcode is HELLO
#define DASH 6										// �ϧO���u����0.3��(50ms*6)

int isUp(){
	return (adc_result > 2000);}
int isDown(){
	return (adc_result < 2000);}
int goUp(){
	return (adc_result - adc_result_last > 2000);}
int goDown(){
	return (adc_result_last - adc_result > 2000);}
int isMatch(){
	int m = 0;
	int k;
	if(n > 19){										// just in case record is too short
		for(k=n-19; k<n+1; k++){
			if(adc_record[k] != passcode[m++]){
				matched = 0;
				return 0;}
			else{continue;}}
		return 1;}
	else{
		matched = 0;
		return 0;}}

void FLASH()
{
	// Only used if running from FLASH
	// Note that the variable FLASH is defined by the compiler (-d FLASH)
	#ifdef FLASH
	// Copy time critical code and Flash setup code to RAM
	// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
	// symbols are created by the linker. Refer to the linker files.
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

	// Call Flash Initialization to setup flash waitstates
	// This function must reside in RAM
		InitFlash();	// Call the flash wrapper init function
	#endif //(FLASH)
}
void PWM()
{
	//===============================================================
	// PWM INITIALISATION to use pin31(PWM1A-DAC) as ADC input source
	//===============================================================
	#define period 30						   // 2MHz when PLL is set to 0xC (60MHz)
											   // 60MHz / 30 = 2MHz

		duty_cycle = 15;					   // Initialize to 50% duty cycle

	  // Time-base registers
	   	EPwm1Regs.TBPRD = period;       	   // Set timer period, PWM frequency = 1 / period
	   	EPwm1Regs.TBPHS.all = 0;			   // Time-Base Phase Register
	   	EPwm1Regs.TBCTR = 0;					   // Time-Base Counter Register
	    EPwm1Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;  // Set Immediate load
	    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count-up mode: used for asymmetric PWM
		EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;	   // Disable phase loading
		EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
		EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
		EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

	   	// Setup shadow register load on ZERO
	   	EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
	   	EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on CTR=Zero

	   	// Set compare values
	   	EPwm1Regs.CMPA.half.CMPA = duty_cycle;    	  // Set duty 50% initially

	   	// Set actions
	   	EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;            // Set PWM1A on Zero event
	   	EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;          // Clear PWM1A on Up-Count CompareA event
}
void ADC()
{
	//=======================
	// ADC INITIALISATION
	//=======================

	    EALLOW;
		AdcRegs.ADCCTL1.bit.ADCREFSEL   = 0; 	// use internal band gap reference
	   	AdcRegs.ADCCTL1.bit.ADCBGPWD	= 1;	// power up band gap
	   	AdcRegs.ADCCTL1.bit.ADCREFPWD	= 1;	// power up reference
	   	AdcRegs.ADCCTL1.bit.ADCPWDN 	= 1;	// power up rest of ADC
		AdcRegs.ADCCTL1.bit.ADCENABLE	= 1;	// enable ADC output
		for(i=0; i<5000; i++){}					// wait 60000 cycles = 1ms (each iteration is 12 cycles)

		AdcRegs.ADCCTL1.bit.INTPULSEPOS	= 1;	// create int pulses 1 cycle prior to output latch

		//EOC = end of conversion event, SOC = start of conversion event
		AdcRegs.INTSEL1N2.bit.INT1SEL = 0;		// ADCCH0 (ADC-A0) EOC causes ADCInterrupt1
		AdcRegs.INTSEL1N2.bit.INT1CONT = 1;		// set ADCInterrupt 1 to auto clr (continuous conversion)
		AdcRegs.INTSEL1N2.bit.INT1E = 1;		// enable ADCInterrupt1; 0=none, 1=ADCInt1, 2=ADCInt2

	   	AdcRegs.ADCINTSOCSEL1.bit.SOC0 = 1;		// ADCInterrupt1 causes SOC0
		AdcRegs.ADCSOC0CTL.bit.CHSEL= 0; 		// convert ADC-A0 (CH0) when SOC0 is received
		AdcRegs.ADCSOC0CTL.bit.ACQPS = 6;		// set S/H window to 6 clk cycles (117ns)

		AdcRegs.ADCSOCFRC1.all = 0x0001;  		// kick start ADC by causing an ADCInterrupt1 event
		EDIS;

	//=================================
	//	INTERRUPT INITIALISATION (not needed in this example)
	//  (best to run this section after other initialisation)
	//=================================

	// Enable Peripheral, global Ints and higher priority real-time debug events:
	//	IER |= M_INT3;
	//	EINT;   // Enable Global interrupt INTM
	//	ERTM;   // Enable Global realtime interrupt DBGM
}
void main(void)
{
	DeviceInit();	// Device Life support & GPIO mux settings
//	FLASH();
//	PWM();
	ADC();

	for(n=0; n<1000; n++)
	{
		adc_record[n] = '?';
	}
	n = -1;
	for(;;)
	{
		for(i=0; i<250000; i++){}					// �C����50ms�AŪADC �]��goUp/goDown�ݭn�ɶ�
		adc_result_last = adc_result;
		adc_result = AdcResult.ADCRESULT0;			// between 0x0000 and 0x0FFF (0 to 3 Volts)

		if(goUp()){
			adc_loop = 0;
			n++;
			if(adc_record[n] == '_'){
				n++;}}
		if(goDown()){
			adc_loop = 0;}
		if(isUp()){
			adc_record[n] = 'o';
			if(adc_loop > DASH){
				adc_record[n] = 'O';}}
		if(isDown()){
			if(adc_loop > DASH*2){					// ���j�r����0.6��(0.3s*2)
				adc_record[n+1] = '_';}}
		if(isMatch()){
			if(matched++){}
			else{GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;}}
		adc_loop++;
	}												
}
