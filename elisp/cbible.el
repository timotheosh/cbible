;;; cbible.el --- Minor mode to be used with cbible.

;; Copyright (C) 2015 Free Software Foundation, Inc.
;;
;; Author: Tim Hawes <tim@easyfreeunix.com>
;; Maintainer: Tim Hawes <tim@easyfreeunix.com>
;; Created: 27 Jul 2015
;; Version: 0.01
;; Keywords libsword bible commentary

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

;;; Commentary:

;;

;; Put this file into your load-path and the following into your ~/.emacs:
;;   (require 'cbible)

;;; Code:

(eval-when-compile
  (require 'cl))

(defgroup cbible nil
  "cbible"
  :group 'cbible
  :prefix "cb-")

(defcustom bibleversion "KJV"
  "Bible version to use (as defined within Sword)."
  :type 'string
  :group 'cbible
  )

(defun cbible-reference (version reference)
  (if version
      (setq bibver version)
    (setq bibver bibleversion))
  (substring
   (shell-command-to-string
    (format "cbible -b %s -r \"%s\"" bibver reference))
   0 -1))

(defun cbible-lookup()
  (setq version (read-from-minibuffer "Bible Version: "))
  (setq ref (read-from-minibuffer "Reference: "))
  (insert (cbible-reference version ref)))

(define-minor-mode cbible-mode
  "cbible mode"
  :group 'cbible
  (if cbible-mode
      ))


(provide 'cbible)
;;; cbible.el ends here
