library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.textio.all;
use ieee.std_logic_textio.all;
USE ieee.std_logic_arith.ALL;

entity execution_unit_tb is
end execution_unit_tb;

architecture behave of execution_unit_tb is

  component ripple is
    generic (N : natural := 64);
    port (
      A, B  : in  std_logic_vector(N-1 downto 0);
      Cin   : in  std_logic;
      S     : out std_logic_vector(N-1 downto 0);
      Cout  : out std_logic;
      Ovfl  : out std_logic
    );
  end component ripple;

  constant TestVectorFile : string := "..\Simulation\Adder00.tvs";
  constant PreStimTime : time := 1 ns;
  constant PostStimTime : time := 10 ns;

  file file_VECTORS : text;
  constant c_WIDTH : natural := 64;
  signal r_A, r_B  : std_logic_vector(c_WIDTH-1 downto 0) := (others => '0');
  signal r_Cin     : std_logic;
  signal w_Sum     : std_logic_vector(c_WIDTH-1 downto 0);
  signal w_Cout    : std_logic;
  signal w_Ovfl    : std_logic;

begin

  CARRY_SELECT_ADDER_INST : ripple
    generic map (N => c_WIDTH)
    port map (
      A     => r_A,
      B     => r_B,
      Cin   => r_Cin,
      S     => w_Sum,
      Cout  => w_Cout,
      Ovfl  => w_Ovfl
    );

  process
    variable v_ILINE      : line;
    variable MeasurementIndex : integer := 1;  
    variable v_A          : std_logic_vector(c_WIDTH-1 downto 0);
    variable v_B          : std_logic_vector(c_WIDTH-1 downto 0);
    variable v_Cin        : std_logic;
    variable v_Sum        : std_logic_vector(c_WIDTH-1 downto 0);
    variable v_Cout, v_Ovfl : std_logic;
    variable v_HexA, v_HexB, v_HexSum :  std_logic_vector( c_WIDTH-1 downto 0);  
    variable v_Space      : character;
    variable delay_S, delay_Status : time;

  begin

  file_open(file_VECTORS, TestVectorFile, read_mode);


  while not endfile(file_VECTORS) loop
      readline(file_VECTORS, v_ILINE);
      hread(v_ILINE,v_HexA);
		hread(v_ILINE,v_Hexb);
		read(v_ILINE, v_Cin); 
		hread(v_ILINE, v_sum);
      read(v_ILINE, v_Cout);        
      read(v_ILINE, v_Ovfl);        


      r_A <= (others => 'X');
      r_B <= (others => 'X');
      r_Cin <= 'X';
		
      wait for PreStimTime;

        -- ask aswin about the conversion      

		  
		r_A <= v_HexA;
      r_B <= v_Hexb;
		r_Cin <= v_Cin;


      wait for PreStimTime;
		
      wait on w_Sum;
		
      delay_S := now;

      
      wait on w_Cout, w_Ovfl;
      delay_Status := now;


      wait for PostStimTime;

      MeasurementIndex := MeasurementIndex + 1;

    end loop;


    

    report "All test vectors processed successfully.";
file_close(file_VECTORS);


    wait;

  end process;

end architecture behave;