//////////////////////////////////////////////////
// Top level module for Fishingrod
// 128 bits input and key length
module  Fishingrod  ( start , inp , key , ck , ready , out ) ;


input   start ;
input   [0:15]  inp ;  // Data input
input   [0:15]  key ;  // Key input
input   ck ; // Rising edge clock

output  ready ;
output  [0:15]  out ;  // Data output

wire    round0 ;
wire    [0:4]  round ; //5-bit register for 18 rounds
wire    [0:15]  rout;
wire    [0:4]  sels;
wire    [0:4]  selk;


//( start , ck , rn , round0 ,  round, active , ready, sels, selk )
Fishingrod_control  u_control  (
  .start   ( start   ) ,
  .ck      ( ck      ) ,
  .round0  ( round0  ) ,
  .round   ( round   ) ,
  .ready   ( ready   ) ,
  .sels    ( sels    ) ,
  .selk    ( selk    ) 
) ;

//( inp, key, round0, round, sels, selk, out, clk, rst) 
Fishingrod_comb  u_comb  (
  .inp     ( inp    ), 
  .key     ( key    ), 
  .round0  ( round0 ), 
  .ck      ( ck     ) ,
  .round   ( round  ),
  .sels    ( sels   ),
  .selk    ( selk   ), 
  .out     ( rout   )
) ;

assign  out  =  rout ;


endmodule
