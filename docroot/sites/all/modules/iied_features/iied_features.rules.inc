<?php

/**
 * Implementation of hook_rules_condition_info()
 *
 * Two custom Rules conditions
 *
 * 1. The current path is a node page and has a NID
 * 2. The current node has a translation in the current language
 */
function iied_features_rules_condition_info() {
    return array(
        'current_url_path_has_node' => array(
            'label' => t('Current URL path has node'),
            'group' => t('IIED'),
            'parameter' => array(
                'path' => array('type' => 'text', 'label' => t('The path being viewed')),
            ),
        ),
        'node_has_current_language' => array(
            'label' => t('Node has translation in language'),
            'group' => t('IIED'),
            'parameter' => array(
                'node' => array('type' => 'node', 'label' => t('The node being viewed')),
                'langcode' => array('type' => 'token', 'label' => t('The current language')),
            ),
        ),
    );
}

// Rules condition callback to check if current path has a Node
function current_url_path_has_node($path) {
    // use "!!" to cast possible nid (number) to TRUE (boolean) value
    return !!__get_nid_from_path_string($path);
}

// Utility callback to get numeric NID from path
function __get_nid_from_path_string($path) {
    $pathParts = explode('/', $path);
    if (!isset($pathParts[0]) || $pathParts[0] !== 'node') {
        return FALSE;
    }
    if (!isset($pathParts[1]) || !is_numeric($pathParts[1])) {
        return FALSE;
    }
    return $pathParts[1];
}

// Rules condition callback to check if node has translation
function node_has_current_language($node, $langcode) {
    if (empty($node->translations)) {
        return FALSE;
    }
    $node_languages = array_keys($node->translations->data);
    return in_array($langcode, $node_languages);
}

/**
 * Implementation of hook_rules_action_info().
 *
 * Two custom Rules actions
 *
 * 1. Redirect to default language if current language is not default
 * 2. Load node from NID argument in URL path (if on a node page)
 */
function iied_features_rules_action_info() {
    return array(
        'redirect_to_language_neutral_path' => array(
            'label' => t('Redirect to language-neutral path'),
            'group' => t('IIED'),
            'arguments' => array(
                'path' => array('type' => 'text', 'label' => t('The path being viewed')),
                'langcode' => array('type' => 'token', 'label' => t('The current language')),
            ),
            'module' => 'iied_features',
        ),
        'load_node_from_current_url' => array(
            'label' => t('Load node from current URL'),
            'group' => t('IIED'),
            'arguments' => array(
                'path' => array('type' => 'text', 'label' => t('The path being viewed')),
            ),
            'provides' => array(
                'node' => array('type' => 'node', 'label' => t('The node being viewed')),
            ),
            'module' => 'iied_features',
        ),
    );
}

// IF current language is not default THEN redirect to the default language
function redirect_to_language_neutral_path($path, $langcode) {
    $default_languages = array('und', language_default('language'));
    if (!in_array($langcode, $default_languages)) {
        drupal_goto($path, array('language' => 'und'));
    }
}

// Load node being viewed from the current path
function load_node_from_current_url($path) {
    if ($nid = __get_nid_from_path_string($path)) {
        $node = node_load($nid);
        return array('node' => $node);
    }
    // error - no node was found in path
    return array();
}
