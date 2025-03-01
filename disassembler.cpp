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
#include "disassembler.hpp"
#include "cartridge.hpp"
#include "navigator.hpp"
#include "symbol_table.hpp"
#include <cstring>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

#include <iostream>
#include <iomanip>

namespace
{
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief CB opcode operations
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static std::string CB_OP[] =
{
  "rlc",
  "rrc",
  "rl",
  "rr",
  "sla",
  "sra",
  "sll",
  "srl"
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief 8-bit registers
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static std::string REG8[] =
{
  "b",
  "c",
  "d",
  "e",
  "h",
  "l",
  "(hl)",
  "a"
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief General opcode texts
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static std::vector <std::string> OPCODE_TEXT =
{
  "nop",
  "ld\tbc,%word%",
  "ld\t(bc),a",
  "inc\tbc",
  "inc\tb",
  "dec\tb",
  "ld\tb,%byte%",
  "rlca",
  "ex\taf,af'",
  "add\thl,bc",
  "ld\ta,(bc)",
  "dec\tbc",
  "inc\tc",
  "dec\tc",
  "ld\tc,%byte%",
  "rla",
  "djnz\t%reladdr%",
  "ld\tde,%word%",
  "ld\t(de),a",
  "inc\tde",
  "inc\td",
  "dec\td",
  "ld\td,%byte%",
  "rrca",
  "jr\t%reladdr%",
  "add\thl,de",
  "ld\ta,(de)",
  "dec\tde",
  "inc\te",
  "dec\te",
  "ld\te,%byte%",
  "rra",
  "jr\tnz,%reladdr%",
  "ld\thl,%word%",
  "ld\t(%addr%),hl",
  "inc\thl",
  "inc\th",
  "dec\th",
  "ld\th,%byte%",
  "daa",
  "jr\tz,%reladdr%",
  "add\thl,hl",
  "ld\thl,(%addr%)",
  "dec\thl",
  "inc\tl",
  "dec\tl",
  "ld\tl,%byte%",
  "cpl",
  "jr\tnc,%reladdr%",
  "ld\tsp,%word%",
  "ld\t(%addr%),a",
  "inc\tsp",
  "inc\t(hl)",
  "dec\t(hl)",
  "ld\t(hl),%byte%",
  "scf",
  "jr\tc,%reladdr%",
  "add\thl,sp",
  "ld\ta,(%addr%)",
  "dec\tsp",
  "inc\ta",
  "dec\ta",
  "ld\ta,%byte%",
  "ccf",
  "ld\tb,b",
  "ld\tb,c",
  "ld\tb,d",
  "ld\tb,e",
  "ld\tb,h",
  "ld\tb,l",
  "ld\tb,(hl)",
  "ld\tb,a",
  "ld\tc,b",
  "ld\tc,c",
  "ld\tc,d",
  "ld\tc,e",
  "ld\tc,h",
  "ld\tc,l",
  "ld\tc,(hl)",
  "ld\tc,a",
  "ld\td,b",
  "ld\td,c",
  "ld\td,d",
  "ld\td,e",
  "ld\td,h",
  "ld\td,l",
  "ld\td,(hl)",
  "ld\td,a",
  "ld\te,b",
  "ld\te,c",
  "ld\te,d",
  "ld\te,e",
  "ld\te,h",
  "ld\te,l",
  "ld\te,(hl)",
  "ld\te,a",
  "ld\th,b",
  "ld\th,c",
  "ld\th,d",
  "ld\th,e",
  "ld\th,h",
  "ld\th,l",
  "ld\th,(hl)",
  "ld\th,a",
  "ld\tl,b",
  "ld\tl,c",
  "ld\tl,d",
  "ld\tl,e",
  "ld\tl,h",
  "ld\tl,l",
  "ld\tl,(hl)",
  "ld\tl,a",
  "ld\t(hl),b",
  "ld\t(hl),c",
  "ld\t(hl),d",
  "ld\t(hl),e",
  "ld\t(hl),h",
  "ld\t(hl),l",
  "halt",
  "ld\t(hl),a",
  "ld\ta,b",
  "ld\ta,c",
  "ld\ta,d",
  "ld\ta,e",
  "ld\ta,h",
  "ld\ta,l",
  "ld\ta,(hl)",
  "ld\ta,a",
  "add\ta,b",
  "add\ta,c",
  "add\ta,d",
  "add\ta,e",
  "add\ta,h",
  "add\ta,l",
  "add\ta,(hl)",
  "add\ta,a",
  "adc\ta,b",
  "adc\ta,c",
  "adc\ta,d",
  "adc\ta,e",
  "adc\ta,h",
  "adc\ta,l",
  "adc\ta,(hl)",
  "adc\ta,a",
  "sub\tb",
  "sub\tc",
  "sub\td",
  "sub\te",
  "sub\th",
  "sub\tl",
  "sub\t(hl)",
  "sub\ta",
  "sbc\ta,b",
  "sbc\ta,c",
  "sbc\ta,d",
  "sbc\ta,e",
  "sbc\ta,h",
  "sbc\ta,l",
  "sbc\ta,(hl)",
  "sbc\ta,a",
  "and\tb",
  "and\tc",
  "and\td",
  "and\te",
  "and\th",
  "and\tl",
  "and\t(hl)",
  "and\ta",
  "xor\tb",
  "xor\tc",
  "xor\td",
  "xor\te",
  "xor\th",
  "xor\tl",
  "xor\t(hl)",
  "xor\ta",
  "or\tb",
  "or\tc",
  "or\td",
  "or\te",
  "or\th",
  "or\tl",
  "or\t(hl)",
  "or\ta",
  "cp\tb",
  "cp\tc",
  "cp\td",
  "cp\te",
  "cp\th",
  "cp\tl",
  "cp\t(hl)",
  "cp\ta",
  "ret\tnz",
  "pop\tbc",
  "jp\tnz,%addr%",
  "jp\t%addr%",
  "call\tnz,%addr%",
  "push\tbc",
  "add\ta,%byte%",
  "rst\t00h",
  "ret\tz",
  "ret",
  "jp\tz,%addr%",
  "",
  "call\tz,%addr%",
  "call\t%addr%",
  "adc\ta,%byte%",
  "rst\t08h",
  "ret\tnc",
  "pop\tde",
  "jp\tnc,%addr%",
  "out\t(%byte%),a",
  "call\tnc,%addr%",
  "push\tde",
  "sub\t%byte%",
  "rst\t10h",
  "ret\tc",
  "exx",
  "jp\tc,%addr%",
  "in\ta,(%byte%)",
  "call\tc,%addr%",
  "",
  "sbc\ta,%byte%",
  "rst\t18h",
  "ret\tpo",
  "pop\thl",
  "jp\tpo,%addr%",
  "ex\t(sp),hl",
  "call\tpo,%addr%",
  "push\thl",
  "and\t%byte%",
  "rst\t20h",
  "ret\tpe",
  "jp\t(hl)",
  "jp\tpe,%addr%",
  "ex\tde,hl",
  "call\tpe,%addr%",
  "",
  "xor\t%byte%",
  "rst\t28h",
  "ret\tp",
  "pop\taf",
  "jp\tp,%addr%",
  "di",
  "call\tp,%addr%",
  "push\taf",
  "or\t%byte%",
  "rst\t30h",
  "ret\tm",
  "ld\tsp,hl",
  "jp\tm,%addr%",
  "ei",
  "call\tm,%addr%",
  "",
  "cp\t%byte%",
  "rst\t38h",
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief DD/FD xx opcode texts
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::vector <std::string> OPCODE_DDFD_TEXT =
{
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "add\t%regw%,bc",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "add\t%regw%,de",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\t%regw%,%word%",
  "ld\t(%addr%),%regw%",
  "inc\t%regw%",
  "inc\t%regw%h *",
  "dec\t%regw%h *",
  "ld\t%regw%,%byte%",
  "nop (2x) *",
  "nop (2x) *",
  "add\t%regw%,%regw%",
  "ld\t%regw%,(%addr%)",
  "dec\t%regw%",
  "inc\t%regw%l *",
  "dec\t%regw%l *",
  "ld\t%regw%l,%byte%",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "inc\t(%regw% + %byte%)",
  "dec\t(%regw% + %byte%)",
  "ld\t(%regw% + %byte%),%byte%",
  "nop (2x) *",
  "nop (2x) *",
  "add\t%regw%,sp",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\tb,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\tc,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\td,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\te,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\th,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\tl,(%regw% + %byte%)",
  "nop (2x) *",
  "ld\t(%regw% + %byte%),b",
  "ld\t(%regw% + %byte%),c",
  "ld\t(%regw% + %byte%),d",
  "ld\t(%regw% + %byte%),e",
  "ld\t(%regw% + %byte%),h",
  "ld\t(%regw% + %byte%),l",
  "nop (2x) *",
  "ld\t(%regw% + %byte%),a",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\ta,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "add\ta,%regw%h *",
  "add\ta,%regw%l *",
  "add\ta,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "adc\ta,%regw%h *",
  "adc\ta,%regw%l *",
  "adc\ta,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "sub\t%regw%h *",
  "sub\t%regw%l *",
  "sub\t(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "sbc\ta,%regw%h *",
  "sbc\ta,%regw%l *",
  "sbc\ta,(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "and\t%regw%h *",
  "and\t%regw%l *",
  "and\t(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "xor\t%regw%h *",
  "xor\t%regw%l *",
  "xor\t(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "or\t%regw%h *",
  "or\t%regw%l *",
  "or\t(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "cp\t%regw%h *",
  "cp\t%regw%l *",
  "cp\t(%regw% + %byte%)",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "pop\t%regw%",
  "nop (2x) *",
  "ex\t(sp),%regw%",
  "nop (2x) *",
  "push\t%regw%",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "jp\t%regw%",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ld\tsp,%regw%",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief ED xx opcode texts
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::vector <std::string> OPCODE_ED_TEXT =
{
  "in\tb,(c)",          // 0x40
  "out\t(c),b",
  "sbc\thl,bc",
  "ld\t(%addr%),bc",
  "neg",
  "ret\tn",
  "im\t0",
  "ld\ti,a",
  "in\tc,(c)",
  "out\t(c),c",
  "adc\thl,bc",
  "ld\tbc,(%addr%)",
  "neg *",
  "ret\ti *",
  "im\t0/1 *",
  "ld\tr,a",
  "in\td,(c)",
  "out\t(c),d",
  "sbc\thl,de",
  "ld\t(%addr%),de",
  "neg *",
  "ret\tn *",
  "im\t1",
  "ld\ta,i",
  "in\te,(c)",
  "out\t(c),e",
  "adc\thl,de",
  "ld\tde,(%addr%)",
  "neg *",
  "ret\ti *",
  "im\t2",
  "ld\ta,r",
  "in\th,(c)",
  "out\t(c),h",
  "sbc\thl,hl",
  "ld\t(%addr%),hl",
  "neg *",
  "ret\tn *",
  "im\t0 *",
  "rrd",
  "in\tl,(c)",
  "out\t(c),l",
  "adc\thl,hl",
  "ld\thl,(%addr%)",
  "neg *",
  "ret\ti *",
  "im\t0/1 *",
  "rld",
  "in\t(hl),(c)",
  "out\t(c),(hl)",
  "sbc\thl,sp",
  "ld\t(%addr%),sp",
  "neg *",
  "ret\tn *",
  "im\t1 *",
  "nop (2x) *",
  "in\ta,(c)",
  "out\t(c),a",
  "adc\thl,sp",
  "ld\tsp,(%addr%)",
  "neg *",
  "ret\ti *",
  "im\t2 *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ldi",
  "cpi",
  "ini",
  "outi",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ldd",
  "cpd",
  "ind",
  "outd",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "ldir",
  "cpir",
  "inir",
  "otir",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "lddr",
  "cpdr",
  "indr",
  "otdr",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
  "nop (2x) *",
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Convert uint8_t value to hexa
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static std::string
to_hex (std::uint8_t v)
{
  char buffer[4];
  sprintf (buffer, "%02x", v);

  return buffer;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Convert uint16_t value to hexa
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
static std::string
to_hex (std::uint16_t v)
{
  char buffer[8];
  sprintf (buffer, "%04x", v);

  return buffer;
}

} // namespace

namespace msxdasm
{
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Disassembler implementation class
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
class disassembler::impl
{
public:
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Datatypes
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  using addr_type = cartridge::addr_type;

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Get start address
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  addr_type
  get_start_address () const
  {
    return cartridge_.get_start_address ();
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Get end address
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  addr_type
  get_end_address () const
  {
    return cartridge_.get_end_address ();

  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Get execution address
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  addr_type
  get_exec_address () const
  {
    return cartridge_.get_exec_address ();
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Set execution address
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  void
  set_exec_address (addr_type addr)
  {
    cartridge_.set_exec_address (addr);
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  //! \brief Load symbol file (.def)
  //! \param addr Address
  //! \param label Label
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  void
  load_def (const std::string& path)
  {
    symbols_.load_def (path);
  }

  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  // Prototypes
  // =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
  void add_entry_point (addr_type);
  std::string get_opcode_text (addr_type) const;
  std::string get_opcode_text_cb (addr_type) const;
  std::string get_opcode_text_ddfd (addr_type) const;
  std::string get_opcode_text_ed (addr_type) const;
  std::string get_symbol (addr_type) const;
  std::string format_opcode_text (const std::string&, addr_type, const std::string& = {}) const;
  void load_rom (const std::string&, addr_type);
  void navigate ();
  void generate (const std::string&);
  void generate_asm_code (const std::string&);
  void generate_asm_listing (const std::string&);

private:
  //! \brief Cartridge object
  cartridge cartridge_;

  //! \brief Code navigator
  navigator navigator_;

  //! \brief Symbol list
  symbol_table symbols_;
};

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Add entry point
//! \param addr Address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::impl::add_entry_point (addr_type addr)
{
  navigator_.add_entry_point (addr);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get opcode text
//! \param pc Address
//! \return Opcode text
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
disassembler::impl::get_opcode_text (addr_type pc) const
{
  std::string text;

  std::uint8_t opcode = cartridge_.get_byte (pc);

  switch (opcode)
    {
      case 0xcb: text = get_opcode_text_cb (pc); break;
      case 0xed: text = get_opcode_text_ed (pc); break;
      case 0xdd:
      case 0xfd: text = get_opcode_text_ddfd (pc); break;
      default:   text = format_opcode_text (OPCODE_TEXT[opcode], pc + 1);
    }

  return text;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get symbol
//! \param ref Address
//! \return Symbol
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
disassembler::impl::get_symbol (addr_type ref) const
{
  std::string symbol;

  if (symbols_.has_symbol (ref))
    symbol = symbols_.get_label (ref);

  else if (navigator_.is_entry_point (ref))
    symbol = 'L' + to_hex (ref);
    
  else
    symbol = to_hex (ref) + 'h';
    
  return symbol;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Format opcode text
//! \param fmt_text Format string
//! \param pc Address
//! \param regw Reg-16 name (either ix or iy)
//! \return Opcode text
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
disassembler::impl::format_opcode_text (
  const std::string& fmt_text,
  addr_type pc,
  const std::string& regw
) const
{
  std::string text;

  // replace %var% with corresponding values
  std::size_t pos = 0;
  std::size_t pct_pos = fmt_text.find ('%', pos);

  while (pct_pos != std::string::npos)
    {
      text += fmt_text.substr (pos, pct_pos - pos);
      auto end_pos = fmt_text.find ('%', pct_pos + 1);

      std::string var = fmt_text.substr (pct_pos + 1, end_pos - pct_pos - 1);

      if (var == "addr")
        {
          addr_type ref = cartridge_.get_word (pc);
          text += get_symbol (ref);
          pc += 2;
        }

      else if (var == "reladdr")
        {
          addr_type ref = cartridge_.get_offset (pc);
          text += get_symbol (ref);
          pc++;
        }

      else if (var == "byte")
        {
          text += to_hex (cartridge_.get_byte (pc)) + 'h';
          pc++;
        }

      else if (var == "word")
        {
          text += to_hex (cartridge_.get_word (pc)) + 'h';
          pc += 2;
        }
		
	  else if (var == "regw")
        text += regw;

      pos = end_pos + 1;
      pct_pos = fmt_text.find ('%', pos);
    }

  // add last chars
  text += fmt_text.substr (pos);

  return text;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get opcode text for CB opcode family
//! \param pc Address
//! \return Opcode text
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
disassembler::impl::get_opcode_text_cb (addr_type pc) const
{
  std::string text;
  std::uint8_t opcode = cartridge_.get_byte (pc + 1);
  std::uint8_t key = (opcode >> 6) & 3;
  std::uint8_t op1 = (opcode >> 3) & 7;
  std::uint8_t op2 = (opcode) & 7;

  switch(key)
    {
      case 0: text = CB_OP[op1] + '\t' + REG8[op2]; break;
      case 1: text = "bit\t" + std::to_string (op1) + ',' + REG8[op2]; break;
      case 2: text = "res\t" + std::to_string (op1) + ',' + REG8[op2]; break;
      case 3: text = "set\t" + std::to_string (op1) + ',' + REG8[op2]; break;
    }

  return text;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get opcode text for DD(FD) opcode family
//! \param pc Address
//! \return Opcode text
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
disassembler::impl::get_opcode_text_ddfd (addr_type pc) const
{
  std::string text;

  std::uint8_t opcode = cartridge_.get_byte (pc);
  const std::string regw = (opcode == 0xdd) ? "ix" : "iy";
  
  if (cartridge_.get_byte (pc + 1) == 0xcb)
    {
      std::uint8_t offset = cartridge_.get_byte (pc + 2);
      std::uint8_t opcode = cartridge_.get_byte (pc + 3);
      std::uint8_t op1 = (opcode >> 3) & 7;
      std::uint8_t key = (opcode >> 6) & 3;
      std::string addr = '(' + regw + " + " + std::to_string (offset) + ')';

      switch (key)
        {
          case 0: text = CB_OP[op1] + '\t' + addr; break;
          case 1: text = "bit\t" + std::to_string (op1) + ',' + addr; break;
          case 2: text = "res\t" + std::to_string (op1) + ',' + addr; break;
          case 3: text = "set\t" + std::to_string (op1) + ',' + addr; break;
        }
    }

  else
    {
      const std::string fmt_text = OPCODE_DDFD_TEXT[cartridge_.get_byte (pc + 1)];
      pc += 2;
      text = format_opcode_text (fmt_text, pc, regw);
    }
    
  return text;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get opcode text for ED opcode family
//! \param pc Address
//! \return Opcode text
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
std::string
disassembler::impl::get_opcode_text_ed (addr_type pc) const
{
  std::string text;
  std::uint8_t opcode = cartridge_.get_byte (pc + 1);
  
  if (opcode < 64 || opcode > 191)
    text = "nop (2x) *";
    
  else
    text = format_opcode_text (OPCODE_ED_TEXT[opcode - 64], pc + 2);
    
  return text;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Load .rom file into memory
//! \param path File path
//! \param addr Start address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::impl::load_rom (const std::string& path, addr_type addr)
{
  cartridge_.load_rom (path, addr);

  symbols_.add_symbol (addr, "signtr", "cartridge signature = 'AB'");
  symbols_.add_symbol (addr + 2, "staddr", "start address value");
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigate through code
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::impl::navigate ()
{
  navigator_.navigate (cartridge_);

  auto pc = cartridge_.get_exec_address ();
  symbols_.add_symbol (pc, "start", "execution starting point");
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Generate output, according to file extension
//! \param path File path
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::impl::generate (const std::string& path)
{
  auto pos = path.rfind ('.');
  if (pos == std::string::npos)
    throw std::invalid_argument ("Cannot determine output file format. File has no extension");

  auto ext = path.substr (pos + 1);
  
  if (ext == "asm")
    generate_asm_code (path);
      
  else if (ext == "lst")
    generate_asm_listing (path);
      
  else
    throw std::invalid_argument ("Invalid output file format");
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Generate .asm code file
//! \param path File path
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::impl::generate_asm_code (const std::string& path)
{
  std::ofstream out (path);
  if (!out)
    throw std::system_error (errno, std::system_category (), "Failed to open file");

  addr_type start_addr = cartridge_.get_start_address ();
  addr_type end_addr = cartridge_.get_end_address ();
  addr_type pc = start_addr;

  out << "\t\t\torg\t" << to_hex (start_addr) << 'h' << std::endl;

  while (pc <= end_addr)
    {
      std::uint16_t ref = 0;

      if (symbols_.has_symbol (pc))
        {
          auto label = symbols_.get_label (pc);
          auto comment = symbols_.get_comment (pc);

          out << '\n' << label << ':';

          if (!comment.empty ())
            out << "\t\t\t\t\t\t; " << comment;

          out << '\n';
        }

      out << "\t\t\t";

      if (navigator_.is_db (pc))
        {
          out << "db\t" << to_hex (cartridge_.get_byte (pc)) << 'h';
          pc++;
          int i = 0;

          while (i < 7 && pc <= end_addr && navigator_.is_db (pc))
            {
              out << ',' << to_hex (cartridge_.get_byte (pc)) << 'h';
              pc++;
              i++;
            }
        }

      else if (navigator_.is_string (pc))
        {
          out << "db\t\"";

          while (navigator_.is_string (pc))
            {
              out << cartridge_.get_byte (pc);
              ++pc;
            }
            
          out << '"';
        }
        
      else if (navigator_.is_dw (pc))
        {
          ref = cartridge_.get_word (pc);
          out << "dw\t" << symbols_.get_label (ref);
          pc = pc + 2;
        }

      else if (navigator_.is_code (pc))
        {
          out << get_opcode_text (pc);
          pc += navigator_.get_opcode_size (pc);
        }

      out << '\n';
    }

  out.close ();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Generate .asm listing file, complete with addresses and opcodes
//! \param path File path
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::impl::generate_asm_listing (const std::string& path)
{
  std::ofstream out (path);
  if (!out)
    throw std::system_error (errno, std::system_category (), "Failed to open file");

  addr_type start_addr = cartridge_.get_start_address ();
  addr_type end_addr = cartridge_.get_end_address ();
  addr_type pc = start_addr;

  out << "\t\t\torg\t" << to_hex (start_addr) << 'h' << std::endl;

  while (pc <= end_addr)
    {
      std::uint16_t ref = 0;

      if (symbols_.has_symbol (pc))
        {
          auto label = symbols_.get_label (pc);
          auto comment = symbols_.get_comment (pc);

          out << '\n' << label << ':';

          if (!comment.empty ())
            out << "\t\t\t\t\t\t; " << comment;

          out << '\n';
        }

      else if (navigator_.is_entry_point (pc))
        {
          out << "\nL" << to_hex (pc) << ':' << '\n';
        }

      out << to_hex (pc) << '\t';

      if (navigator_.is_db (pc))
        {
          out << "\t\tdb\t" << to_hex (cartridge_.get_byte (pc)) << 'h';
          pc++;
          int i = 0;

          while (i < 7 && pc <= end_addr && navigator_.is_db (pc))
            {
              out << ',' << to_hex (cartridge_.get_byte (pc)) << 'h';
              pc++;
              i++;
            }
        }

      else if (navigator_.is_string (pc))
        {
          out << "\t\tdb\t\"";

          while (navigator_.is_string (pc))
            {
              out << cartridge_.get_byte (pc);
              ++pc;
            }
            
          out << '"';
        }
        
      else if (navigator_.is_dw (pc))
        {
          ref = cartridge_.get_word (pc);
          out << to_hex (cartridge_.get_byte (pc)) << ' ' << to_hex (cartridge_.get_byte (pc + 1))
              << "\t\tdw\t" << get_symbol (ref);
          pc += 2;
        }

      else if (navigator_.is_code (pc))
        {
          auto siz = navigator_.get_opcode_size (pc);

          for (std::uint16_t i = 0;i < siz;i++)
            out << to_hex (cartridge_.get_byte (pc + i)) << ' ';

          for (std::uint16_t i = siz; i < 4;i++)
            out << "   ";

          out << '\t' << get_opcode_text (pc);
          pc += navigator_.get_opcode_size (pc);
        }

      out << '\n';
    }

  out.close ();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Constructor
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
disassembler::disassembler ()
  : impl_ (std::make_shared <impl> ())
{
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get start address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
disassembler::addr_type
disassembler::get_start_address () const
{
  return impl_->get_start_address ();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get end address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
disassembler::addr_type
disassembler::get_end_address () const
{
  return impl_->get_end_address ();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Get execution address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
disassembler::addr_type
disassembler::get_exec_address () const
{
  return impl_->get_exec_address ();
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Set execution address
//! \param addr Address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::set_exec_address (addr_type addr)
{
  impl_->set_exec_address (addr);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Add entry point
//! \param addr Address
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::add_entry_point (addr_type addr)
{
  impl_->add_entry_point (addr);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Load .rom file
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::load_rom (const std::string& path, addr_type addr)
{
  impl_->load_rom (path, addr);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Load .def file
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::load_def (const std::string& path)
{
  impl_->load_def (path);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Generate output file
//! \param path File path
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::generate (const std::string& path)
{
  impl_->generate (path);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Generate .asm listing file, complete with addresses and opcodes
//! \param path File path
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::generate_asm_listing (const std::string& path)
{
  impl_->generate_asm_listing (path);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Generate .asm code file
//! \param path File path
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::generate_asm_code (const std::string& path)
{
  impl_->generate_asm_code (path);
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//! \brief Navigate code
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void
disassembler::navigate ()
{
  impl_->navigate ();
}

} // namespace msxdasm