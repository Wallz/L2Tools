import ida_struct
import ida_bytes

last_struc_idx = get_last_struc_idx()

# newId = add_struc(last_struc_idx + 1, "TestStruct", False)
# print get_struc_size(newId)
# add_struc_member(newId, "TestFieldName", BADADDR, FF_WORD, ida_struct.MF_OK, 0);
# newStruct = ida_struct.get_struc(newId)
# print ida_struct.get_member_by_name(newStruct, "TestFieldName").flag

print get_struc_size(newId)
# add_struc_member(newId, "TestFieldName", BADADDR, FF_WORD, ida_struct.MF_OK, 0);
newStruct = ida_struct.get_struc(newId)
print ida_struct.get_member_by_name(newStruct, "field_0").flag
print ida_struct.get_member_by_name(newStruct, "field_0").props