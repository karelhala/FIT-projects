-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2011 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Zdenek Vasicek <vasicek AT fit.vutbr.cz>
--				Karel Hala <xhalak AT stud.fit.vutbr.cz>

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is
	type fsm_state is(sfetch,sidle,sdecode,shalt,si_val,sd_val,begin_while,while_zero,load_while,while_continue,go_to_begin,end_while,sput,sload);
	type instruction is(i_ptr,d_ptr,i_val,d_val,l_bra,r_bra,put,load,halt,jump);

	signal pstate : fsm_state;
	signal nstate : fsm_state;
	signal decode : instruction;

	
	signal pc_reg : std_logic_vector (11 downto 0);
	signal pc_inc: std_logic;
	signal pc_dec: std_logic;
	
	signal cnt_reg: std_logic_vector (7 downto 0);
	signal cnt_inc: std_logic;
	signal cnt_dec: std_logic;
	
	signal ptr_reg: std_logic_vector (9 downto 0); 
	signal ptr_inc: std_logic;
	signal ptr_dec: std_logic;

 -- zde dopiste potrebne deklarace signalu

begin
	CODE_ADDR <= pc_reg;
	DATA_ADDR <= ptr_reg;
	
	-- ----------------------------------------
	-- 					PC					 --
	-- ----------------------------------------
	function_pc: process(RESET, CLK, pc_inc, pc_dec)
	begin
		if (RESET='1') then
			pc_reg <= "000000000000";
		elsif (CLK'event) and (CLK='1') then
			if (pc_inc='1') then
				pc_reg<=pc_reg + 1;
			elsif (pc_dec='1') then
				pc_reg<=pc_reg-1;
			end if;
		end if;
		
	end process;

	-- ----------------------------------------
	-- 					CNT					 --
	-- ----------------------------------------
	function_cnt: process(RESET, CLK, cnt_inc, cnt_dec)
	begin
		if (RESET='1') then
			cnt_reg <= "00000000";
		elsif (CLK'event) and (CLK='1') then
			if (cnt_inc='1') then
				cnt_reg<=cnt_reg + 1;
			elsif (cnt_dec='1') then
				cnt_reg<=cnt_reg-1;
			end if;
		end if;
	end process;
	
	-- ----------------------------------------
	-- 					PTR					 --
	-- ----------------------------------------
	function_ptr: process(RESET, CLK, ptr_inc, ptr_dec)
	begin
		if (RESET='1') then
			ptr_reg <= "0000000000";
		elsif (CLK'event) and (CLK='1') then
			if (ptr_inc='1') then
				ptr_reg<=ptr_reg + 1;
			elsif (ptr_dec='1') then
				ptr_reg<=ptr_reg-1;
			end if;
		end if;
	end process;
	-- ----------------------------------------
	-- 					FSM					 --
	-- ----------------------------------------
	
	fsm_pstate: process (RESET,CLK)
	begin
		if (RESET='1') then
			pstate <= sidle;
		elsif (CLK'event) and (CLK='1') then
			if (EN = '1') then
				pstate <=nstate;
			end if;
		end if;
	end process;
	
	
	-- ----------------------------------------
	-- 					DECODER				 --
	-- ----------------------------------------
	
	fsm_nstate: process (CODE_DATA)
	begin
		if (CODE_DATA=X"3E") then
			decode <= i_ptr;			-- >
		elsif(CODE_DATA=X"3C") then
			decode <= d_ptr;			-- <
		elsif(CODE_DATA=X"2B") then
			decode <= i_val;			-- +
		elsif (CODE_DATA=X"2D") then
			decode <= d_val;			-- -
		elsif (CODE_DATA=X"5B") then
			decode <= l_bra;			-- [
		elsif (CODE_DATA=X"5D") then
			decode <= r_bra;			-- ]
		elsif (CODE_DATA=X"2E") then
			decode <= put;				-- .
		elsif (CODE_DATA=X"2C") then
			decode <= load;				-- ,
		elsif (CODE_DATA=X"00") then
			decode <= halt;				-- konec
		else
		decode <= jump;					-- komentare
	end if;
	end process;
	
	-- ----------------------------------------
	-- 				Konecny automat			 --
	-- ----------------------------------------
	function_fsm: process(CODE_DATA, DATA_RDATA, EN, IN_DATA, IN_VLD, OUT_BUSY,pstate,decode)
	begin
	
		-- nastaveni jednotlyvich stavu a promennych
		nstate<=sidle;
		
		DATA_RDWR<='0';
		DATA_WDATA<=X"00";
		
		-- vynulovani vystupnich signalu
		OUT_WE<='0';
		IN_REQ<='0';
		DATA_EN<='0';
		CODE_EN<='0';
		
		-- vynulovani PC
		pc_dec<='0';
		pc_inc<='0';
		
		-- vynulovani pocitadla
		cnt_inc<='0';
		cnt_dec<='0';

		-- vynulovani ukazatele
		ptr_inc<='0'; 
		ptr_dec<='0';
		
		case pstate is 
			-- IDLE
			when sidle =>
				nstate<=sfetch;
				
			-- FETCH
			when sfetch =>
				CODE_EN<='1';
				nstate<= sdecode;
			-- DECODE
			when sdecode =>
				case decode is
				
					-- inst HALT
					when halt =>
						nstate <= shalt;
						
					-- inst >
					when i_ptr =>
						pc_inc<='1';
						ptr_inc<='1';
						nstate <=sidle;
						
					-- inst <
					when d_ptr =>
						ptr_dec<='1';
						pc_inc<='1';
						nstate <=sidle;
						
					-- inst +
					when i_val =>
						DATA_EN<='1';
						DATA_RDWR<='0';
						
						nstate <= si_val;
					
					-- inst -
					when d_val =>
						DATA_EN<='1';
						DATA_RDWR<='0';
						
						nstate <= sd_val;
						
					-- inst .
					when put =>
						DATA_EN<='1';
						DATA_RDWR<='0';
						
						nstate<=sput;
					
					-- inst ,
					when load =>
						IN_REQ<='1';
						nstate <=sload;
								
					-- inst [			
					when l_bra =>
						DATA_EN<='1';
						DATA_RDWR<='0';
						nstate <=begin_while;
					
					-- inst ]
					when r_bra =>
						DATA_EN<='1';
						DATA_RDWR<='0';
						nstate <=end_while;
					
					-- vsechno ostatni jsou komentare
					when jump=>
						nstate<=sidle;
						pc_inc<='1';
					
					-- Konec
					when others =>
						pc_inc<='1';
				end case;
				
			-- zacatek cyklu ([)
			when begin_while =>
				if (DATA_RDATA=0) then
					pc_inc<='1';
					cnt_inc<='1';
					nstate<=while_continue;
				else
					pc_inc<='1';
					nstate<=sidle;
				end if;
			
			-- preskoceni cyklu na konec
			when while_zero =>
				if (decode=r_bra) then
					pc_inc<='1';
					nstate<=sidle;
				else
					pc_inc<='1';
					nstate<=while_continue;
				end if;
				
			-- pro zpomaleni cyklu
			when while_continue=>
				CODE_EN<='1';
				nstate<=while_zero;
				
			-- konec cyklu (])
			when end_while =>
				if (DATA_RDATA=0) then
					pc_inc<='1';
					nstate<=sidle;
				else
					pc_dec<='1';
					nstate<=load_while;
				end if;
				
			-- preskoceni cyklu na zacatek
			when go_to_begin =>
				if (decode=l_bra) then
					nstate<=sidle;
				else
					pc_dec<='1';
					nstate<=load_while;
				end if;
				
			-- zpomaleni cyklu
			when load_while =>
				CODE_EN<='1';
				nstate<=go_to_begin;
				
			-- nacitani
			when sload =>
				if (IN_VLD='1') then
					
					DATA_WDATA <= IN_DATA;
					DATA_RDWR<='1';
					DATA_EN<='1';
					
					pc_inc <= '1';
					nstate <=sidle;
				else
					IN_REQ<='1';
					nstate<=sload;
				end if;
			
			-- zvysovani hodnoty
			when si_val =>
				DATA_WDATA <= DATA_RDATA + 1;
				DATA_RDWR <= '1';
				DATA_EN <= '1';

				pc_inc <= '1';
				nstate <=sidle;
				
			-- snizovani hodnoty
			when sd_val =>

				DATA_WDATA<=DATA_RDATA - 1;
				DATA_EN<='1';
				DATA_RDWR<='1';
				
				pc_inc <= '1';
				nstate <=sidle;
				
			-- vypis
			when sput =>
				if (OUT_BUSY='0') then
					OUT_DATA<=DATA_RDATA;
					
					OUT_WE<='1';
					pc_inc<='1';
					nstate<=sidle;
				else
					DATA_EN<='1';
					DATA_RDWR<='0';
						
					nstate<=sput;
				end if;
				
			-- konec
			when shalt=>
				nstate<=shalt;
			when others =>
		end case;
	end process;

end behavioral;

