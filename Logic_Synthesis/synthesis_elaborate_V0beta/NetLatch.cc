// NetLatch.cc
// Author: Alan M. Feldstein
// Class NetLatch member-function definitions

#include "NetLatch.h" // NetLatch class definition
#include "target.h" // target_t structure definition

// constructor
NetLatch::NetLatch( NetScope *scope, perm_string name, unsigned width )
  // explicitly call base-class constructor
  : NetNode( scope, name, 2U * width + 3U )
{
	pin_Gate().set_dir(Link::INPUT);
	pin_Gate().set_name(perm_string::literal("I"), 0);

	for(unsigned idx = 0 ;  idx < width ;  idx += 1){
		pin_Data(idx).set_dir(Link::INPUT);
		pin_Data(idx).set_name(perm_string::literal("I"), idx);

		pin_Q(idx).set_dir(Link::OUTPUT);
		pin_Q(idx).set_name(perm_string::literal("O"), idx);
	}
} // end NetLatch constructor

unsigned NetLatch::width() const
{
  return ( pin_count() - 3U ) / 2U;
} // end function width

Link &NetLatch::pin_Data( unsigned w )
{
  unsigned pn = 3 + 2 * w;
  assert( pn < pin_count() );
  return pin( pn );
} // end function pin_Data

/*start------ADU*/
const Link &NetLatch::pin_Data( unsigned w ) const
{
  unsigned pn = 3 + 2 * w;
  assert( pn < pin_count() );
  return pin( pn );
} // end function pin_Data
/*end------ADU*/

Link &NetLatch::pin_Q( unsigned w )
{
  unsigned pn = 4 + 2 * w;
  assert( pn < pin_count() );
  return pin( pn );
} // end function pin_Q

/*start------ADU*/
const Link &NetLatch::pin_Q( unsigned w ) const
{
  unsigned pn = 4 + 2 * w;
  assert( pn < pin_count() );
  return pin( pn );
} // end function pin_Q
/*end------ADU*/

Link &NetLatch::pin_Gate()
{
  return pin( 0 );
} // end function pin_Gate

const Link &NetLatch::pin_Gate() const
{
  return pin( 0 );
} // end function pin_Gate

//ADU:
Link &NetLatch::pin_Aclr()
{
	return pin( 1 );
} // end function pin_Aclr

const Link &NetLatch::pin_Aclr() const
{
	return pin( 1 );
} // end function pin_Aclr

Link &NetLatch::pin_Aset()
{
	return pin( 2 );
} // end function pin_Aset

const Link &NetLatch::pin_Aset() const
{
	return pin( 2 );
} // end function pin_Aset

bool NetLatch::emit_node( target_t *driverPtr ) const
{
  driverPtr->lpm_latch( this );
  return true;
} // end function emit_node
