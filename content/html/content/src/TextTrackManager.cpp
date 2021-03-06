/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set tw=80 expandtab softtabstop=2 ts=2 sw=2: */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/TextTrackManager.h"
#include "mozilla/dom/HTMLMediaElement.h"

namespace mozilla {
namespace dom {

NS_IMPL_CYCLE_COLLECTION_1(TextTrackManager, mTextTracks)
NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(TextTrackManager, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(TextTrackManager, Release)

TextTrackManager::TextTrackManager(HTMLMediaElement *aMediaElement)
  : mMediaElement(aMediaElement)
{
  MOZ_COUNT_CTOR(TextTrackManager);
  mTextTracks = new TextTrackList(mMediaElement->OwnerDoc()->GetParentObject());
}

TextTrackManager::~TextTrackManager()
{
  MOZ_COUNT_DTOR(TextTrackManager);
}

TextTrackList*
TextTrackManager::TextTracks() const
{
  return mTextTracks;
}

already_AddRefed<TextTrack>
TextTrackManager::AddTextTrack(TextTrackKind aKind, const nsAString& aLabel,
                               const nsAString& aLanguage)
{
  return mTextTracks->AddTextTrack(mMediaElement, aKind, aLabel, aLanguage);
}

void
TextTrackManager::AddTextTrack(TextTrack* aTextTrack)
{
  mTextTracks->AddTextTrack(aTextTrack);
}

void
TextTrackManager::RemoveTextTrack(TextTrack* aTextTrack)
{
  mTextTracks->RemoveTextTrack(aTextTrack);
}

void
TextTrackManager::DidSeek()
{
  mTextTracks->DidSeek();
}

void
TextTrackManager::Update(double aTime)
{
  mTextTracks->Update(aTime);
}

} // namespace dom
} // namespace mozilla
