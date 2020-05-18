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

(defun cbible-reference (reference &optional version)
  "Function for looking up Bible passages. If version is ommitted,
   it will use default version."
  (if (and version (> (length version) 0))
      (setq bibver version)
    (setq bibver bibleversion))
  (substring
   (shell-command-to-string
    (format "cbible -b %s -r \"%s\"" bibver reference))
   0 -1))

(defun cbible-lookup()
  "Function that prompts for Scripture reference and version."
  (interactive)
  (setq version (read-from-minibuffer "Bible Version: "))
  (setq ref (read-from-minibuffer "Reference: "))
  (insert (cbible-reference ref version)))

(defun cbible-make-entry(entry)
  "Function for creating a Personal commentary entry."
  (setq ref (read-from-minibuffer "Reference: "))
  (shell-command
   (format "echo \"%s\"|cbible -b Personal -r \"%s\" -i" entry ref)))

(defun cbible-entry-region()
  "Send region as a Personal commentary entry."
  (interactive)
  (cbible-make-entry (buffer-substring-no-properties (region-beginning) (region-end))))

(defun cbible-entry-buffer()
  (interactive)
  "Send entire buffer as a Personal commentary entry."
  (cbible-make-entry (buffer-substring-no-properties 1 (buffer-size))))

(define-minor-mode cbible-mode
  "cbible mode allows you to insert Bible quotes using Crosswire
   Bible software. It interacts with a small program called cbible
   to access libsword Bibles (see http://crosswire.org for more
   information)."
  :lighter " cbible "
  :keymap (let ((map (make-sparse-keymap)))
            (define-key map (kbd "C-c l") 'cbible-lookup)
            map)
  :group 'cbible
      )

(provide 'cbible)
;;; cbible.el ends here
