
/*
  +------------------------------------------------------------------------+
  | Phalcon Framework                                                      |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#include "validation/validator/between.h"
#include "validation/validator.h"
#include "validation/validatorinterface.h"
#include "validation/message.h"
#include "validation/exception.h"
#include "validation.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/concat.h"

/**
 * Phalcon\Validation\Validator\Url
 *
 * Checks if a value has a correct URL format
 *
 *<code>
 *use Phalcon\Validation\Validator\Url as UrlValidator;
 *
 *$validator->add('url', new UrlValidator(array(
 *   'message' => 'The url is not valid'
 *)));
 *</code>
 */
zend_class_entry *phalcon_validation_validator_url_ce;

PHP_METHOD(Phalcon_Validation_Validator_Url, validate);

static const zend_function_entry phalcon_validation_validator_url_method_entry[] = {
	PHP_ME(Phalcon_Validation_Validator_Url, validate, arginfo_phalcon_validation_validatorinterface_validate, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Phalcon\Validation\Validator\Url initializer
 */
PHALCON_INIT_CLASS(Phalcon_Validation_Validator_Url){

	PHALCON_REGISTER_CLASS_EX(Phalcon\\Validation\\Validator, Url, validation_validator_url, phalcon_validation_validator_ce, phalcon_validation_validator_url_method_entry, 0);

	zend_class_implements(phalcon_validation_validator_url_ce TSRMLS_CC, 1, phalcon_validation_validatorinterface_ce);

	return SUCCESS;
}

/**
 * Executes the validation
 *
 * @param Phalcon\Validation $validator
 * @param string $attribute
 * @return boolean
 */
PHP_METHOD(Phalcon_Validation_Validator_Url, validate){

	zval *validator, *attribute, *value, *validate_url;
	zval *validation, *message_str, *message, *code;
	zval *allow_empty, *label, *pairs, *prepared;
	zend_class_entry *ce = Z_OBJCE_P(getThis());

	PHALCON_MM_GROW();

	phalcon_fetch_params(1, 2, 0, &validator, &attribute);
	
	phalcon_fetch_params(1, 2, 0, &validator, &attribute);

	PHALCON_VERIFY_CLASS_EX(validator, phalcon_validation_ce, phalcon_validation_exception_ce, 1);

	PHALCON_OBS_VAR(value);
	phalcon_call_method_p1_ex(value, &value, validator, "getvalue", attribute);
	
	PHALCON_INIT_VAR(allow_empty);
	RETURN_MM_ON_FAILURE(phalcon_validation_validator_getoption_helper(ce, allow_empty, getThis(), "allowEmpty" TSRMLS_CC));
	if (zend_is_true(allow_empty) && phalcon_validation_validator_isempty_helper(value)) {
		RETURN_MM_TRUE;
	}

	PHALCON_ALLOC_GHOST_ZVAL(validate_url);
	ZVAL_LONG(validate_url, 273);
	
	PHALCON_OBS_VAR(validation);
	phalcon_call_func_p2_ex(validation, &validation, "filter_var", value, validate_url);
	if (!zend_is_true(validation)) {
	
		PHALCON_INIT_VAR(label);
		RETURN_MM_ON_FAILURE(phalcon_validation_validator_getoption_helper(ce, label, getThis(), "label" TSRMLS_CC));
		if (!zend_is_true(label)) {
			PHALCON_CPY_WRT(label, attribute);
		}

		PHALCON_ALLOC_GHOST_ZVAL(pairs);
		array_init_size(pairs, 1);
		Z_ADDREF_P(label); add_assoc_zval_ex(pairs, SS(":field"), label);

		PHALCON_INIT_VAR(message_str);
		RETURN_MM_ON_FAILURE(phalcon_validation_validator_getoption_helper(ce, message_str, getThis(), "message" TSRMLS_CC));
		if (!zend_is_true(message_str)) {
			PHALCON_INIT_NVAR(message_str);
			RETURN_MM_ON_FAILURE(phalcon_validation_getdefaultmessage_helper(Z_OBJCE_P(validator), message_str, validator, "Url" TSRMLS_CC));
		}
	
		PHALCON_INIT_VAR(code);
		RETURN_MM_ON_FAILURE(phalcon_validation_validator_getoption_helper(ce, code, getThis(), "code" TSRMLS_CC));
		if (Z_TYPE_P(code) == IS_NULL) {
			ZVAL_LONG(code, 0);
		}

		PHALCON_OBS_VAR(prepared);
		phalcon_call_func_p2_ex(prepared, &prepared, "strtr", message_str, pairs);

		message = phalcon_validation_message_construct_helper(prepared, attribute, "Url", code TSRMLS_CC);
		Z_DELREF_P(message);
	
		phalcon_call_method_p1_noret(validator, "appendmessage", message);
		RETURN_MM_FALSE;
	}
	
	RETURN_MM_TRUE;
}