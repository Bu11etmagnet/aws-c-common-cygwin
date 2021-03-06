/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <aws/common/byte_buf.h>
#include <proof_helpers/make_common_data_structures.h>

void aws_byte_buf_eq_c_str_ignore_case_harness() {
    /* parameters */
    struct aws_byte_buf buf;
    const char *c_str = make_arbitrary_c_str(MAX_BUFFER_SIZE);

    /* assumptions */
    __CPROVER_assume(aws_byte_buf_is_bounded(&buf, MAX_BUFFER_SIZE));
    ensure_byte_buf_has_allocated_buffer_member(&buf);
    __CPROVER_assume(aws_byte_buf_is_valid(&buf));

    /* save current state of the parameters */
    struct aws_byte_buf old = buf;
    struct store_byte_from_buffer old_byte;
    save_byte_from_array(buf.buffer, buf.len, &old_byte);
    size_t str_len = (c_str) ? strlen(c_str) : 0;
    struct store_byte_from_buffer old_byte_from_str;
    save_byte_from_array((uint8_t *)c_str, str_len, &old_byte_from_str);

    /* operation under verification */
    if (aws_byte_buf_eq_c_str_ignore_case(&buf, c_str)) {
        assert(buf.len == str_len);
    }

    /* asserts both parameters remain unchanged */
    assert(aws_byte_buf_is_valid(&buf));
    assert_byte_buf_equivalence(&buf, &old, &old_byte);
    if (str_len > 0) {
        assert_byte_from_buffer_matches((uint8_t *)c_str, &old_byte_from_str);
    }
}
