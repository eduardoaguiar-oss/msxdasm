// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// MSXDasm
// Copyright (C) 1999-2025 Eduardo Aguiar
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#include "navigator.hpp"
#include "cartridge.hpp"
#include <queue>
#include <set>

#include <iostream>

namespace
{
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Opcode sizes. 0 for DD, ED, FD
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static constexpr std::uint8_t OPCODE_SIZE[256] =
{
  1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,       // 00-0f
  2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1,       // 10-1f
  2, 3, 3, 1, 1, 1, 2, 1, 2, 1, 3, 1, 1, 1, 2, 1,       // 20-2f
  2, 3, 3, 1, 1, 1, 2, 1, 2, 1, 3, 1, 1, 1, 2, 1,       // 30-3f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // 40-4f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // 50-5f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // 60-6f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // 70-7f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // 80-8f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // 90-9f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // a0-af
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // b0-bf
  1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 2, 3, 3, 2, 1,       // c0-cf
  1, 1, 3, 2, 3, 1, 2, 1, 1, 1, 3, 2, 3, 0, 2, 1,       // d0-df
  1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 0, 2, 1,       // e0-ef
  1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 0, 2, 1,       // f0-ff
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief DD and FD opcode sizes
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static constexpr std::uint8_t OPCODE_SIZE_DDFD[256] =
{
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,       // 00-0f
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,       // 10-1f
  1, 4, 4, 2, 2, 2, 3, 1, 1, 2, 4, 2, 2, 2, 3, 1,       // 20-2f
  1, 1, 1, 1, 3, 3, 4, 1, 1, 2, 1, 1, 1, 1, 1, 1,       // 30-3f
  1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 2, 2, 3, 1,       // 40-4f
  1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 2, 2, 3, 1,       // 50-5f
  2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 3, 2,       // 60-6f
  3, 3, 3, 3, 3, 3, 1, 3, 1, 1, 1, 1, 2, 2, 3, 1,       // 70-7f
  1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 2, 2, 3, 1,       // 80-8f
  1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 2, 2, 3, 1,       // 90-9f
  1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 2, 2, 3, 1,       // a0-af
  1, 1, 1, 1, 2, 2, 3, 1, 1, 1, 1, 1, 2, 2, 3, 1,       // b0-bf
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1,       // c0-cf
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,       // d0-df
  1, 2, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,       // e0-ef
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1,       // f0-ff
};

} // namespace

namespace msxdasm
{
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigator implementation class
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class navigator::impl
{
private:
  // Memory status type
  enum status { STATUS_UNKNOWN, STATUS_DB, STATUS_DW, STATUS_STRING, STATUS_CODE };

  //! \brief Entry points found
  std::set <addr_type> entry_points_;

  //! \brief Entry points to navigate
  std::queue <addr_type> entry_points_queue_;

  //! \brief Cartridge object
  cartridge cartridge_;

  //! \brief Memory map
  std::array <status, 0x10000> memory_map_;

  //! \brief swtcha function address
  addr_type swtcha_ = 0;

public:
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Constructors
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  impl () = default;
  impl (const impl&) = delete;
  impl (impl&&) = delete;

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Operators
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  impl& operator= (const impl&) = delete;
  impl& operator= (impl&&) = delete;

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Check if address content is DB (single byte)
  //! \param pc Memory pos
  //! \return true/false
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool
  is_db (addr_type pc) const
  {
      return memory_map_.at (pc) == STATUS_DB;
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Check if address content is DW (single word)
  //! \param pc Memory pos
  //! \return true/false
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool
  is_dw (addr_type pc) const
  {
      return memory_map_.at (pc) == STATUS_DW;
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Check if address content is STRING
  //! \param pc Memory pos
  //! \return true/false
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool
  is_string (addr_type pc) const
  {
      return memory_map_.at (pc) == STATUS_STRING;
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Check if address content is CODE
  //! \param pc Memory pos
  //! \return true/false
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool
  is_code (addr_type pc) const
  {
      return memory_map_.at (pc) == STATUS_CODE;
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Check if address is entry point
  //! \param pc Memory pos
  //! \return true/false
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  bool
  is_entry_point (addr_type pc) const
  {
      return entry_points_.find (pc) != entry_points_.end ();
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Prototypes
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  std::uint8_t get_opcode_size (addr_type) const;
  void set_status (addr_type, std::uint16_t, status);
  void add_entry_point (addr_type);
  void navigate (const cartridge&);
  void navigate_branch (addr_type);
  std::uint8_t navigate_opcode (addr_type);
  void detect_swtcha ();
  void navigate_swtcha (addr_type);
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Add entry point to the navigation queue
//! \param pc Address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::impl::add_entry_point (addr_type pc)
{
  entry_points_queue_.push (pc);
  entry_points_.insert (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Detect swtcha function (Konami)
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::impl::detect_swtcha ()
{
  auto pc = cartridge_.get_start_address ();
  auto end_addr = cartridge_.get_end_address ();

  while (!swtcha_ && pc < end_addr - 5)
    {
      if (cartridge_.get_byte (pc) == 0x87 &&
          cartridge_.get_byte (pc + 1) == 0xe1 &&
          cartridge_.get_byte (pc + 2) == 0xcd &&
          cartridge_.get_byte (pc + 5) == 0xe9)
        swtcha_ = pc;
      ++pc;
    }

  if (swtcha_)
      add_entry_point (swtcha_);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Set memory range status
//! \param pc Memory pos
//! \param size Size in bytes
//! \param st Status
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::impl::set_status (addr_type pc, std::uint16_t size, status st)
{
    if (pc + size < pc) // overflow
      size = 0xffff - size + 1;

    std::fill (memory_map_.data () + pc, memory_map_.data () + pc + size, st);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get opcode size
//! \param pc Address
//! \return Opcode size in bytes
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::uint8_t
navigator::impl::get_opcode_size (addr_type pc) const
{
  std::uint8_t opcode = cartridge_.get_byte (pc);
  std::uint8_t siz = OPCODE_SIZE[opcode];

  if (!siz)
    {
      std::uint8_t operand = cartridge_.get_byte (pc + 1);

      if (opcode == 0xed)
        siz = ((operand & 0xc7) == 0x43) ? 4 : 2;

      else              // 0xdd and 0xfd
        siz = OPCODE_SIZE_DDFD[operand];
    }

  return siz;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigate through code
//! \param cart Cartridge object
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::impl::navigate (const cartridge& cart)
{
  cartridge_ = cart;
  memory_map_.fill (STATUS_UNKNOWN);

  auto start_addr = cartridge_.get_start_address ();
  auto end_addr = cartridge_.get_end_address ();

  // Set cartridge header status
  set_status (start_addr, 2, STATUS_STRING);    // 'AB' signature
  set_status (start_addr + 2, 2, STATUS_DW);    // Execution entry point

  // Cartridge execution point
  add_entry_point (cartridge_.get_exec_address ());

  // Search for swtcha function
  detect_swtcha ();
 
  // Navigate through code until the navigation queue is empty
  while (!entry_points_queue_.empty ())
    {
      auto pc = entry_points_queue_.front ();
      entry_points_queue_.pop ();
      
      if (pc >= start_addr && pc <= end_addr)
        navigate_branch (pc);
    }
    
  // Mark unknown addresses as DB
  for (addr_type i = start_addr;i <= end_addr;i++)
    {
      if (memory_map_[i] == STATUS_UNKNOWN)
          memory_map_[i] = STATUS_DB;
    }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigate one code branch, gathering jump addresses
//! \param pc Address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::impl::navigate_branch (addr_type pc)
{
  if (memory_map_[pc] != STATUS_UNKNOWN)
      return;

  auto end_addr = cartridge_.get_end_address ();

  while (pc <= end_addr)
    {
      std::uint8_t siz = navigate_opcode (pc);

      if (siz)
          pc += siz;
      else
          return;
    }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigate one opcode at time
//! \param pc Address
//! \return Opcode size or 0 to end this branch navigation
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::uint8_t
navigator::impl::navigate_opcode (addr_type pc)
{
  std::uint8_t opcode = cartridge_.get_byte (pc);
  std::uint8_t siz = get_opcode_size (pc);
  set_status (pc, siz, STATUS_CODE);
  addr_type ref;

  switch (opcode)
    {
      case 0x10:                                // djnz xx
        ref = cartridge_.get_offset (pc+1);
        add_entry_point (ref);
        break;

      case 0x18:                                // jr xx
        ref = cartridge_.get_offset (pc+1);
        add_entry_point (ref);
        return 0;
        break;
        
      case 0xc3:                                // jp xxxx
        ref = cartridge_.get_word (pc+1);
        add_entry_point (ref);
        return 0;
        break;

      case 0xc9:                                // ret
        return 0;
        break;

      case 0xcd:                                // call
        ref = cartridge_.get_word (pc+1);
        
        if (ref == swtcha_)
          {
            navigate_swtcha (pc);
            return 0;
          }

        else
          add_entry_point (ref);

        break;

      case 0xdd:                                // ix
      case 0xfd:                                // iy
        if (cartridge_.get_byte (pc + 1) == 0xe9)   // jp (ix/iy)
          return 0;
        break;

      case 0xe9:                                // jp (hl)
        return 0;
        break;
        
      case 0xc4:                                // call cc
      case 0xcc:
      case 0xd4:
      case 0xdc:
      case 0xe4:
      case 0xec:
      case 0xf4:
      case 0xfc:
        ref = cartridge_.get_word (pc+1);
        add_entry_point (ref);
        break;

      case 0xc2:                                // jp cc
      case 0xca:
      case 0xd2:
      case 0xda:
      case 0xe2:
      case 0xea:
      case 0xf2:
      case 0xfa:
        ref = cartridge_.get_word (pc+1);
        add_entry_point (ref);
        break;

      case 0x20:                                // jr cc
      case 0x28:
      case 0x30:
      case 0x38:
        ref = cartridge_.get_offset (pc+1);
        add_entry_point (ref);
        break;
    }

  return siz;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigate through SWTCHA code structure
//! \param pc Address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::impl::navigate_swtcha (addr_type pc)
{
  pc += 3;
  addr_type addr_end = cartridge_.get_word (pc);

  while (pc < addr_end)
    {
      addr_type ref = cartridge_.get_word (pc);
      add_entry_point (ref);
      set_status (pc, 2, STATUS_DW);
      pc += 2;
    }
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Constructor
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
navigator::navigator ()
  : impl_ (std::make_shared <impl> ())
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigate code
//! \param cart Cartridge object
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::navigate (const cartridge& cart)
{
  impl_->navigate (cart);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get opcode size
//! \param pc Address
//! \return Opcode size in bytes
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::uint8_t
navigator::get_opcode_size (addr_type pc) const
{
  return impl_->get_opcode_size (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Check if address content is DB (single byte)
//! \param pc Memory pos
//! \return true/false
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool
navigator::is_db (addr_type pc) const
{
  return impl_->is_db (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Check if address content is DW (single word)
//! \param pc Memory pos
//! \return true/false
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool
navigator::is_dw (addr_type pc) const
{
  return impl_->is_dw (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Check if address content is STRING
//! \param pc Memory pos
//! \return true/false
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool
navigator::is_string (addr_type pc) const
{
  return impl_->is_string (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Check if address content is CODE
//! \param pc Memory pos
//! \return true/false
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool
navigator::is_code (addr_type pc) const
{
  return impl_->is_code (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Check if address is entry point
//! \param pc Memory pos
//! \return true/false
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
bool
navigator::is_entry_point (addr_type pc) const
{
  return impl_->is_entry_point (pc);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Add entry point to the navigation queue
//! \param pc Address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
navigator::add_entry_point (addr_type pc)
{
  impl_->add_entry_point (pc);
}

} // namespace msxdasm