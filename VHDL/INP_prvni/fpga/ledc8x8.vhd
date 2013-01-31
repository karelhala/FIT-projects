library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use IEEE.std_logic_arith.all;

entity ledc8x8 is
port( SMCLK: in std_logic;
      RESET: in std_logic;
      ROW: out std_logic_vector(7 downto 0);
      LED: out std_logic_vector(7 downto 0)
); end ledc8x8;

architecture behv of ledc8x8 is

signal ctrl: std_logic_vector(21 downto 0);	-- promenna pro 22b citacku
signal rowon: std_logic_vector(7 downto 0);	-- promenna pro ukladani, ktery radek je aktivni
signal ledon: std_logic_vector(7 downto 0);	--promenna pro ukladani, ktera ledka je aktivni
signal ce: std_logic;	-- prepinani radku, ktery bude aktivni
signal switch: std_logic;	-- prepinac vypnuteho/zapnuteho displaze
begin


-- proces pro 22b citacku 
proc_cnt_clk: process(SMCLK, RESET,ctrl,ce)
begin
	if RESET = '1' then -- pri signalu reset, vynuluji citacku
		ctrl <= "0000000000000000000000";
	elsif SMCLK'event and SMCLK = '1' then --postupne pricitam jednicku
		ctrl <=  ctrl + "0000000000000000000001";
		switch <= ctrl(21); -- nejvyssi bit citacky nacitam do switch, ktery ovlada vypinani diplaye

		if ctrl(7 downto 0)="11111111" then
			ce <= '1'; -- pokud spodnich 8b jsou same jednicky nastavim signal ce, ktery ovlada prepinani radku, na jedna
		else 
			ce <= '0'; -- jinak je ce nastaven na 0 = neprepinam radek
		end if;
	end if;

end process proc_cnt_clk;
 
-- proces pro aktivaci radku
row_cnt: process(ce, RESET, rowon,SMCLK)
begin
	if RESET = '1' then -- pokud vyresetujeme, nastavime aktivitu radku na prvni radek
		rowon <= "11111110";
	elsif SMCLK'event and SMCLK='1' and ce = '1' then	-- pokud program zaregistruje zmenu hodin a signal ce je nastaven na 1, prepnu aktivni radek
		rowon <=  rowon(6 downto 0) & rowon(7);
	end if;
end process row_cnt;


-- proces pro urcovani, ktere ledky budou svitit v kterem radku
proc_led_dc: process(ce,rowon,ledon,switch,SMCLK)
begin
	if SMCLK'event and SMCLK = '1' then
		case rowon is
			when "11111110" =>	ledon <= "00001001";	-- prvni radek	
			when "11111101" => ledon <= "00000101";		-- druhy radek
			when "11111011" => ledon <= "00000011";		-- treti radek
			when "11110111" => ledon <= "10010101";		-- ctvrty radek
			when "11101111" => ledon <= "10011001";		-- paty radek
			when "11011111" => ledon <= "11110000";		-- sesty radek
			when "10111111" => ledon <= "10010000";		-- sedmy radek
			when "01111111" => ledon <= "10010000";		-- osmy radek
			when others => ledon <= "00000000";
		end case;
	end if;
end process proc_led_dc;


-- proces pro rizeni vypnuti, nebo zaptnuti displaye
proc_multiplex: process(switch,ledon,SMCLK,rowon)
begin
	if SMCLK'event and SMCLK = '1' then		-- pokud je zaregistrovana zmena hodin
		ROW <= rowon;		-- nastavim uz primo radek, ktery bude svitit
		if switch = '0' then	-- pokud je switch 0, tak zapnu display
			LED <=ledon;		-- jinak ho vypnu
		else
			LED <= "00000000";
		end if;
	end if;
end process proc_multiplex;

end architecture;
